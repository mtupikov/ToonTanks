#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmunitionBase.generated.h"

class UDamageType;

UCLASS()
class TOWEROFFENCE_API AAmmunitionBase : public AActor {
	GENERATED_BODY()

public:
	AAmmunitionBase();

	float GetFireRate() const;
	const FVector& GetSpawnPoint() const;

	virtual void Detonate();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* HitParticle = nullptr;

	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* HitSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* LaunchSound = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float FireRate = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float Damage = 50.0f;

private:
	FVector SpawnPoint;
};
