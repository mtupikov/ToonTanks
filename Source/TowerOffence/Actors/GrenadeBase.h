#pragma once

#include "CoreMinimal.h"
#include "AmmunitionBase.h"
#include "GrenadeBase.generated.h"

UCLASS()
class TOWEROFFENCE_API AGrenadeBase : public AAmmunitionBase {
	GENERATED_BODY()

public:
	AGrenadeBase();
	void SetThrowPower(float Power);

	virtual void Detonate() override;

	UFUNCTION()
	void OnHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		FVector NormalImpulse,
		const FHitResult& Hit
	);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* GrenadeMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float AreaDamage = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float AreaDamageDistance = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float LifeSpanTime = 3.0;

	FTimerHandle LifeSpanTimerHandle;
	const float MaxGrenadeSpeed = 2000.0f;
};
