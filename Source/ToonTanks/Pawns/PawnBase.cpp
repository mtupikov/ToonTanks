#include "PawnBase.h"

#include "Components/CapsuleComponent.h"
#include "ToonTanks/Actors/ProjectileBase.h"

APawnBase::APawnBase() {
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComponent;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void APawnBase::BeginPlay() {
	Super::BeginPlay();
}

void APawnBase::RotateTurretToTarget(FVector TargetLocation) {
	const auto LookAtTargetClean = FVector(TargetLocation.X, TargetLocation.Y, TurretMesh->GetComponentLocation().Z);
	const auto StartLocation = TurretMesh->GetComponentLocation();
	const auto TurretRotation = FVector(LookAtTargetClean - StartLocation).Rotation();

	TurretMesh->SetWorldRotation(TurretRotation);
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

void APawnBase::HandleDestruction() {

}

void APawnBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void APawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
