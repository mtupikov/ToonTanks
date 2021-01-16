#include "PawnBase.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "TowerOffence/Actors/ForceFieldBase.h"
#include "TowerOffence/Actors/ProjectileBase.h"
#include "TowerOffence/Components/HealthComponent.h"
#include "TowerOffence/Components/PawnMovementComponentBase.h"
#include "TowerOffence/Components/ShootProjectileComponent.h"
#include "TowerOffence/Components/ShootTraceComponent.h"

APawnBase::APawnBase() {
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = BaseMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	FireSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	FireSpawnPoint->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

	MovementComponent = CreateDefaultSubobject<UPawnMovementComponentBase>(TEXT("Movement Component"));
	MovementComponent->UpdatedComponent = RootComponent;
}

void APawnBase::BeginPlay() {
	Super::BeginPlay();

	InitialRotator = TurretMesh->GetComponentRotation();

	switch (ShootType) {
	case EShootType::ProjectileHomingRocket: {
		ShootComponent = NewObject<UShootProjectileComponent>(this, UShootProjectileComponent::StaticClass(), TEXT("Shoot Homing Projectile Component"));
		Cast<UShootProjectileComponent>(ShootComponent)->SetProjectileIsHoming(true);
		break;
	}
	case EShootType::ProjectileRocket: {
		ShootComponent = NewObject<UShootProjectileComponent>(this, UShootProjectileComponent::StaticClass(), TEXT("Shoot Projectile Component"));
		break;
	}
	case EShootType::TraceBullet: {
		ShootComponent = NewObject<UShootTraceComponent>(this, UShootTraceComponent::StaticClass(), TEXT("Shoot Trace Bullet Component"));
		break;
	}
	}

	if (ShootComponent) {
		ShootComponent->RegisterComponent();

		if (auto* SPC = Cast<UShootProjectileComponent>(ShootComponent)) {
			SPC->SetProjectile(ProjectileClass);
		}
	}

	if (ForceFieldBP.GetDefaultObject()) {
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.bNoFail = true;
		SpawnInfo.Owner = this;
		SpawnInfo.Instigator = nullptr;
		SpawnInfo.bDeferConstruction = false;

		ForceField = GetWorld()->SpawnActor<AForceFieldBase>(ForceFieldBP, SpawnInfo);
		ForceField->SetActorScale3D({4.3, 4.3, 4.3});
		ForceField->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

bool APawnBase::IsAlive() const {
	return bIsPawnAlive;
}

void APawnBase::SetIsAlive(bool Value) {
	if (bIsPawnAlive != Value) {
		bIsPawnAlive = Value;
	}
}

bool APawnBase::ForceFieldIsActive() const {
	return ForceField && ForceField->IsActive();
}

void APawnBase::RotateTurretToTarget(
	const FVector& TargetLocation,
	float LeftMaxAngle,
	float RightMaxAngle,
	bool IgnoreIfOutOfRange
) {
	if (TurretRotationSpeed <= 0.0f) {
		return;
	}

	const auto Rotators = RotatorsToLocation(InitialRotator, TargetLocation, LeftMaxAngle, RightMaxAngle);
	const auto IsInRange = UKismetMathLibrary::InRange_FloatFloat(Rotators.ResultRotator.Yaw, Rotators.LeftMaxRotator.Yaw, Rotators.RightMaxRotator.Yaw, false, false);
	if (IgnoreIfOutOfRange && !IsInRange) {
		return;
	}

	RotateTurret(Rotators.ResultRotator);
}

float APawnBase::GetFireRate() const {
	return FireRate;
}

float APawnBase::GetTurretRotationSpeed() const {
	return TurretRotationSpeed;
}

UStaticMeshComponent* APawnBase::GetTurretMesh() const {
	return TurretMesh;
}

USceneComponent* APawnBase::GetFireSpawnPoint() const {
	return FireSpawnPoint;
}

UPawnMovementComponentBase* APawnBase::GetPawnMovementComponent() const {
	return MovementComponent;
}

UShootComponent* APawnBase::GetShootComponent() const {
	return ShootComponent;
}

void APawnBase::RotateTurret(const FRotator& Rotation) {
	TurretMesh->SetWorldRotation(Rotation);
}

APawnBase::ResultRotators APawnBase::RotatorsToLocation(
	const FRotator& ActorRotator,
	const FVector& TargetLocation,
	float LeftMaxAngle,
	float RightMaxAngle
) {
	const auto StartLocation = GetTurretLocation();
	const auto LookAtTargetClean = FVector(TargetLocation.X, TargetLocation.Y, StartLocation.Z);
	const auto DeltaTime = GetWorld()->DeltaTimeSeconds;
	auto TurretRotator = FRotationMatrix::MakeFromX(LookAtTargetClean - StartLocation).Rotator();
	TurretRotator = FMath::RInterpConstantTo(GetTurretRotation(), TurretRotator, DeltaTime, TurretRotationSpeed);

	FRotator LeftMaxRotator(ActorRotator);
	if (LeftMaxAngle <= MaximumRotationAngle()) {
		LeftMaxRotator.Add(0, -LeftMaxAngle, 0);
	}

	FRotator RightMaxRotator(ActorRotator);
	if (RightMaxAngle <= MaximumRotationAngle()) {
		RightMaxRotator.Add(0, RightMaxAngle, 0);
	}

	if (LeftMaxRotator != ActorRotator && RightMaxRotator != ActorRotator) {
		TurretRotator.Yaw = UKismetMathLibrary::ClampAngle(TurretRotator.Yaw, LeftMaxRotator.Yaw, RightMaxRotator.Yaw);
	}

	return { TurretRotator, LeftMaxRotator, RightMaxRotator };
}

void APawnBase::Fire(USceneComponent* Target) {
	if (!ShootComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Cannot fire, ShootComponent is null"), *GetName());
		return;
	}

	ShootComponent->Fire(FireSpawnPoint->GetComponentLocation(), FireSpawnPoint->GetComponentRotation(), this, Target);
}

FRotator APawnBase::GetTurretRotation() const {
	return TurretMesh->GetComponentRotation();
}

FRotator APawnBase::GetTurretInitialRotation() const {
	return InitialRotator;
}

FVector APawnBase::GetTurretLocation() const {
	return TurretMesh->GetComponentLocation();
}

float APawnBase::MaximumRotationAngle() {
	return 90.0f;
}

void APawnBase::HandleDestruction() {
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticle, GetActorLocation());
}

void APawnBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void APawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
