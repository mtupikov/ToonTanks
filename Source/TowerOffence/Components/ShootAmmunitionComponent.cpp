#include "ShootAmmunitionComponent.h"

#include "TowerOffence/Actors/AmmunitionBase.h"
#include "TowerOffence/Actors/HitscanBase.h"
#include "TowerOffence/Actors/HomingMissleProjectile.h"
#include "TowerOffence/Actors/GrenadeBase.h"
#include "TowerOffence/Utils/WeaponSpreadManager.h"

#include "DrawDebugHelpers.h"

UShootAmmunitionComponent::UShootAmmunitionComponent() {
	WeaponSpreadManager = CreateDefaultSubobject<UWeaponSpreadManager>(TEXT("Weapon Spread Manager"));
}

void UShootAmmunitionComponent::Fire(const FVector& SpawnLocation, const FRotator& SpawnRotation, AActor* Owner, USceneComponent* Target, float Charge) {
	if (!ProjectileClass) {
		UE_LOG(LogTemp, Warning, TEXT("Projectile was not set in %s component"), *GetName());
		return;
	}

	if (!GetWorld()) {
		UE_LOG(LogTemp, Error, TEXT("Cannot GetWorld in %s component"), *GetName());
		return;
	}

	const auto RandomSeed = FMath::Rand();
	FRandomStream WeaponRandomStream(RandomSeed);
	const float SpreadCone = FMath::DegreesToRadians(WeaponSpreadManager->GetSpreadRadius());
	const auto ShootDir = WeaponRandomStream.VRandCone(SpawnRotation.Vector(), SpreadCone, SpreadCone).Rotation();

	//DrawDebugCone(GetWorld(), SpawnLocation, SpawnRotation.Vector(), 3000.0f, SpreadCone, SpreadCone, 32, FColor::Red, false, 5.0f, 0, 1.0f);

	WeaponSpreadManager->OnShotFired();

	if (CachedHitscan) {
		FireHitscan(SpawnLocation, ShootDir);
		return;
	}

	auto* Ammunition = GetWorld()->SpawnActor<AAmmunitionBase>(ProjectileClass, SpawnLocation, ShootDir);
	if (!Ammunition) {
		UE_LOG(LogTemp, Error, TEXT("Cannot spawn AAmmunitionBase class, oops! Owner: %s"), *Owner->GetName());
		return;
	}

	Ammunition->SetOwner(Owner);
	for (const auto& Tag : Owner->Tags) {
		if (Tag == FName("Player") || Tag == FName("Enemy")) {
			Ammunition->Tags.Add(Tag);
		}
	}

	if (auto* Hitscan = Cast<AHitscanBase>(Ammunition)) {
		CachedHitscan = Hitscan;
		FireHitscan(SpawnLocation, ShootDir);
		return;
	}

	if (auto* Grenade = Cast<AGrenadeBase>(Ammunition)) {
		Grenade->SetThrowPower(Charge);
	}

	if (auto* Homing = Cast<AHomingMissleProjectile>(Ammunition)) {
		Homing->SetHomingTarget(Target);
	}
}

TSubclassOf<AAmmunitionBase> UShootAmmunitionComponent::GetAmmunition() const {
	return ProjectileClass;
}

void UShootAmmunitionComponent::SetAmmunition(TSubclassOf<AAmmunitionBase> Projectile) {
	ProjectileClass = Projectile;

	if (auto* Ptr = Projectile.GetDefaultObject()) {
		float MaxShots = 0.0f;
		float SpreadDecrease = 0.0f;
		float SpreadRadiusCoef = 1.0f;

		if (Cast<AHitscanBase>(Ptr)) {
			MaxShots = 5.0f;
			SpreadDecrease = 0.025;
			SpreadRadiusCoef = 2.0f;
		} else if (Cast<AHomingMissleProjectile>(Ptr) || Cast<AMissleProjectile>(Ptr)) {
			CachedHitscan = nullptr;
			SpreadDecrease = 0.01;
			MaxShots = 2.0f;
			SpreadRadiusCoef = 4.0f;
		} else if (Cast<AGrenadeBase>(Ptr)) {
			CachedHitscan = nullptr;
			SpreadDecrease = 0.001;
			MaxShots = 2.0f;
			SpreadRadiusCoef = 4.0f;
		}

		WeaponSpreadManager->SetSpreadDecreaseValue(SpreadDecrease);
		WeaponSpreadManager->SetMaxShots(MaxShots);
		WeaponSpreadManager->SetSpreadRadiusCoef(SpreadRadiusCoef);
	}
}

float UShootAmmunitionComponent::GetFireRate() const {
	return ProjectileClass.GetDefaultObject()->GetFireRate();
}

float UShootAmmunitionComponent::GetFireSpreadRadius() const {
	return WeaponSpreadManager->GetSpreadRadius();
}

void UShootAmmunitionComponent::FireHitscan(const FVector& SpawnLocation, const FRotator& SpawnRotation) {
	CachedHitscan->Fire(SpawnRotation.Vector(), SpawnLocation);
}
