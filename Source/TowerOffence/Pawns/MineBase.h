#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MineBase.generated.h"

class AExplosion;
class ATankBase;
class USoundBase;
class UStaticMeshComponent;
class UParticleSystem;
class UHealthComponent;

UCLASS()
class TOWEROFFENCE_API AMineBase : public APawn {
	GENERATED_BODY()

public:
	AMineBase();

	virtual void Tick(float DeltaTime) override;

	void BlowUp();

protected:
	virtual void BeginPlay() override;

private:
	void SelectPlayerPawn();
	float DistanceToPlayer();

	UFUNCTION()
	void OnHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		FVector NormalImpulse,
		const FHitResult& Hit
	);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* DetonationSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* DetectSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* DetonationParticle = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Explosion", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AExplosion> Explosion;

	UPROPERTY(EditDefaultsOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float DetectDistance = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float DetonationTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float AreaDamage = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float OnHitDamage = 100.0f;

	FTimerHandle DetonationTimerHandle;
	ATankBase* PlayerPawn = nullptr;
	bool bPlayerDetected = false;
};
