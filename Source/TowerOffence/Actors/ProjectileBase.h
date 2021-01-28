#pragma once

#include "CoreMinimal.h"
#include "AmmunitionBase.h"
#include "ProjectileBase.generated.h"

class UMatineeCameraShake;
class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS()
class TOWEROFFENCE_API AProjectileBase : public AAmmunitionBase {
	GENERATED_BODY()

public:
	AProjectileBase();

	FVector GetSpawnPoint() const;

	virtual void DestroyProjectile();

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

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystemComponent* TrailParticle = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<UMatineeCameraShake> CameraHitShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float AreaDamage = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float AreaDamageDistance = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float LifeSpanTime = 3.0;
	
	FTimerHandle LifeSpanTimerHandle;
	FVector SpawnPoint;
};
