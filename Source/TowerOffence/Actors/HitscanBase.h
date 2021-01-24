#pragma once

#include "CoreMinimal.h"
#include "AmmunitionBase.h"
#include "HitscanBase.generated.h"

class UNiagaraSystem;

UCLASS()
class TOWEROFFENCE_API AHitscanBase : public AAmmunitionBase {
	GENERATED_BODY()

public:
	AHitscanBase();

	void Fire(const FVector& AimDirection, const FVector& StartLocation);

protected:
	UPROPERTY(EditAnywhere, Category = "Effects")
	UNiagaraSystem* TrailParticle = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float FireSpread = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Range = 3000.0f;

private:
	TArray<FHitResult> WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const;
	void ProcessInstantHit(
		const TArray<FHitResult>& Impacts,
		const FVector& Origin,
		const FVector& End,
		const FVector& ShootDir,
		float Spread
	);
};
