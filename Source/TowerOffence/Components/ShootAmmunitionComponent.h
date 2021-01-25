#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShootAmmunitionComponent.generated.h"

class AHitscanBase;
class AAmmunitionBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOWEROFFENCE_API UShootAmmunitionComponent : public UActorComponent {
	GENERATED_BODY()

public:
	void Fire(const FVector& SpawnLocation, const FRotator& SpawnRotation, AActor* Owner, USceneComponent* Target = nullptr);

	void SetAmmunition(TSubclassOf<AAmmunitionBase> Projectile);

	float GetFireRate() const;

private:
	void FireHitscan(const FVector& SpawnLocation, const FRotator& SpawnRotation);

	TSubclassOf<AAmmunitionBase> ProjectileClass;
	AHitscanBase* CachedHitscan = nullptr;
};
