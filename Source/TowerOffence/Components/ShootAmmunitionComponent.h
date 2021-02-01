#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShootAmmunitionComponent.generated.h"

class AHitscanBase;
class AAmmunitionBase;
class UWeaponSpreadManager;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOWEROFFENCE_API UShootAmmunitionComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UShootAmmunitionComponent();

	void Fire(const FVector& SpawnLocation, const FRotator& SpawnRotation, AActor* Owner, USceneComponent* Target = nullptr, float Charge = 0.0f);

	TSubclassOf<AAmmunitionBase> GetAmmunition() const;
	void SetAmmunition(TSubclassOf<AAmmunitionBase> Projectile);

	float GetFireRate() const;
	float GetFireSpreadRadius() const;

private:
	void FireHitscan(const FVector& SpawnLocation, const FRotator& SpawnRotation, float Charge);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spread", meta = (AllowPrivateAccess = "true"))
	UWeaponSpreadManager* WeaponSpreadManager = nullptr;

	TSubclassOf<AAmmunitionBase> ProjectileClass;
	AHitscanBase* CachedHitscan = nullptr;
};
