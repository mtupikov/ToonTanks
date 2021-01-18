#include "ShootProjectileComponent.h"

#include "TowerOffence/Actors/ProjectileBase.h"
#include "TowerOffence/Actors/HomingMissleProjectile.h"

void UShootProjectileComponent::Fire(const FVector& SpawnLocation, const FRotator& SpawnRotation, AActor* Owner, USceneComponent* Target) {
	if (!ProjectileClass) {
		UE_LOG(LogTemp, Warning, TEXT("Projectile was not set in %s component"), *GetName());
		return;
	}

	if (!GetWorld()) {
		UE_LOG(LogTemp, Error, TEXT("Cannot GetWorld in %s component"), *GetName());
		return;
	}

	AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation);

	if (!TempProjectile) {
		UE_LOG(LogTemp, Error, TEXT("Cannot spawn AProjectileBase class, oops! Owner: %s"), *Owner->GetName());
		return;
	}

	TempProjectile->SetOwner(Owner);

	if (auto* Homing = Cast<AHomingMissleProjectile>(TempProjectile)) {
		Homing->SetHomingTarget(Target);
	}
}

void UShootProjectileComponent::SetProjectile(TSubclassOf<AProjectileBase> Projectile) {
	ProjectileClass = Projectile;
}

float UShootProjectileComponent::GetFireRate() const {
	return ProjectileClass.GetDefaultObject()->GetFireRate();
}
