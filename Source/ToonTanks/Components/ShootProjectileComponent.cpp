#include "ShootProjectileComponent.h"

#include "ToonTanks/Actors/ProjectileBase.h"

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
	TempProjectile->SetOwner(Owner);

	if (Target && bIsHomingProjectile) {
		TempProjectile->SetHomingTarget(Target);
	}
}

void UShootProjectileComponent::SetProjectile(TSubclassOf<AProjectileBase> Projectile) {
	ProjectileClass = Projectile;
}

void UShootProjectileComponent::SetProjectileIsHoming(bool Value) {
	if (bIsHomingProjectile != Value) {
		bIsHomingProjectile = Value;
	}
}
