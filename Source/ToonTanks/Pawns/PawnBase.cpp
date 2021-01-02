#include "PawnBase.h"

#include "Camera/CameraShake.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "ToonTanks/Actors/ProjectileBase.h"
#include "ToonTanks/Components/HealthComponent.h"
#include "ToonTanks/Components/PawnMovementComponentBase.h"

APawnBase::APawnBase() {
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComponent;
	CapsuleComponent->SetCapsuleHalfHeight(70);
	CapsuleComponent->SetCapsuleRadius(70);
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

	MovementComponent = CreateDefaultSubobject<UPawnMovementComponentBase>(TEXT("Movement Component"));
	MovementComponent->UpdatedComponent = RootComponent;
}

void APawnBase::BeginPlay() {
	Super::BeginPlay();

	InitialRotator = TurretMesh->GetComponentRotation();
}

void APawnBase::RotateTurretToTarget(FVector TargetLocation, float LeftMaxAngle, float RightMaxAngle) {
	if (TurretRotationSpeed <= 0.0f) {
		return;
	}

	const auto StartLocation = TurretMesh->GetComponentLocation();
	const auto LookAtTargetClean = FVector(TargetLocation.X, TargetLocation.Y, StartLocation.Z);
	const auto DeltaTime = GetWorld()->DeltaTimeSeconds;
	auto TurretRotator = FRotationMatrix::MakeFromX(LookAtTargetClean - StartLocation).Rotator();
	TurretRotator = FMath::RInterpConstantTo(TurretMesh->GetComponentRotation(), TurretRotator, DeltaTime, TurretRotationSpeed);
	TurretRotator.Normalize();

	FRotator LeftMaxRotator(InitialRotator);
	if (LeftMaxAngle != MaximumRotationAngle()) {
		LeftMaxRotator.Yaw -= LeftMaxAngle;
		LeftMaxRotator.Normalize();
	}

	FRotator RightMaxRotator(InitialRotator);
	if (LeftMaxAngle != MaximumRotationAngle()) {
		RightMaxRotator.Yaw += RightMaxAngle;
		RightMaxRotator.Normalize();
	}

	if (LeftMaxRotator != InitialRotator && RightMaxRotator != InitialRotator) {
		TurretRotator.Yaw = UKismetMathLibrary::ClampAngle(TurretRotator.Yaw, LeftMaxRotator.Yaw, RightMaxRotator.Yaw);
	}

	TurretMesh->SetWorldRotation(TurretRotator);
}

void APawnBase::Fire() {
	if (!ProjectileClass || !TurretMesh) {
		return;
	}
	
	const auto SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
	const auto SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();
	AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation);
	TempProjectile->SetOwner(this);
}

float APawnBase::MaximumRotationAngle() {
	return 180.0f;
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
