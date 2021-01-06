#include "PawnBase.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "ToonTanks/Components/HealthComponent.h"
#include "ToonTanks/Components/PawnMovementComponentBase.h"

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
	if (IgnoreIfOutOfRange
		&& !UKismetMathLibrary::InRange_FloatFloat(Rotators.ResultRotator.Yaw, Rotators.LeftMaxRotator.Yaw, Rotators.RightMaxRotator.Yaw, false, false)) {
		return;
	}

	RotateTurret(Rotators.ResultRotator);
}

float APawnBase::GetFireRate() const {
	return FireRate;
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
	if (LeftMaxAngle != MaximumRotationAngle()) {
		LeftMaxRotator.Add(0, -LeftMaxAngle, 0);
	}

	FRotator RightMaxRotator(ActorRotator);
	if (RightMaxAngle != MaximumRotationAngle()) {
		RightMaxRotator.Add(0, RightMaxAngle, 0);
	}

	if (LeftMaxRotator != ActorRotator && RightMaxRotator != ActorRotator) {
		TurretRotator.Yaw = UKismetMathLibrary::ClampAngle(TurretRotator.Yaw, LeftMaxRotator.Yaw, RightMaxRotator.Yaw);
	}

	return { TurretRotator, LeftMaxRotator, RightMaxRotator };
}

void APawnBase::Fire() {}

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
