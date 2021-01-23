#include "ShootAmmunitionComponent.h"

#include "TowerOffence/Actors/AmmunitionBase.h"
#include "TowerOffence/Actors/HitscanBase.h"
#include "TowerOffence/Actors/HomingMissleProjectile.h"

void UShootAmmunitionComponent::Fire(const FVector& SpawnLocation, const FRotator& SpawnRotation, AActor* Owner, USceneComponent* Target) {
	if (!ProjectileClass) {
		UE_LOG(LogTemp, Warning, TEXT("Projectile was not set in %s component"), *GetName());
		return;
	}

	if (!GetWorld()) {
		UE_LOG(LogTemp, Error, TEXT("Cannot GetWorld in %s component"), *GetName());
		return;
	}

	if (CachedHitscan) {
		FireHitscan(SpawnLocation, SpawnRotation);
		return;
	}

	auto* Ammunition = GetWorld()->SpawnActor<AAmmunitionBase>(ProjectileClass, SpawnLocation, SpawnRotation);
	Ammunition->SetOwner(Owner);
	if (!Ammunition) {
		UE_LOG(LogTemp, Error, TEXT("Cannot spawn AAmmunitionBase class, oops! Owner: %s"), *Owner->GetName());
		return;
	}

	if (auto* Hitscan = Cast<AHitscanBase>(Ammunition)) {
		CachedHitscan = Hitscan;
		FireHitscan(SpawnLocation, SpawnRotation);
		return;
	}

	if (auto* Homing = Cast<AHomingMissleProjectile>(Ammunition)) {
		Homing->SetHomingTarget(Target);
	}
}

void UShootAmmunitionComponent::SetAmmunition(TSubclassOf<AAmmunitionBase> Projectile) {
	ProjectileClass = Projectile;

	if (!Cast<AHitscanBase>(ProjectileClass.GetDefaultObject())) {
		CachedHitscan = nullptr;
	}
}

float UShootAmmunitionComponent::GetFireRate() const {
	return ProjectileClass.GetDefaultObject()->GetFireRate();
}

void UShootAmmunitionComponent::FireHitscan(const FVector& SpawnLocation, const FRotator& SpawnRotation) {
	CachedHitscan->Fire(SpawnRotation.Vector(), SpawnLocation);
}
