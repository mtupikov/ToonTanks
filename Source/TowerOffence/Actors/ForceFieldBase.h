#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ForceFieldBase.generated.h"

class USphereComponent;
class UCurveFloat;
class UTimelineComponent;
class UForceFieldImpact;
class UHealthComponent;

UCLASS()
class TOWEROFFENCE_API AForceFieldBase : public AActor {
	GENERATED_BODY()

public:
	AForceFieldBase();

	void Activate();
	void Deactivate();

	bool IsActive() const;

protected:
	virtual void BeginPlay() override;

private:
	virtual void Tick(float DeltaTime) override;

	void RemoveFinishedImpact(uint32 Key);
	
	UFUNCTION()
	void OnTakeDamage(
		AActor* DamagedActor,
		float Damage,
		const UDamageType* DamageType,
		AController* InstigatedBy,
		AActor* DamageCauser
	);

	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void TimelineCallback(float Value);

	UFUNCTION()
	void TimelineFinishedCallback();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* ForceFieldCollision = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ForceFieldMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact Settings", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* ImpactFloatCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact Settings", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* DisintegrationFloatCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact Settings", meta = (AllowPrivateAccess = "true"))
	float ImpactRadius = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact Settings", meta = (AllowPrivateAccess = "true"))
	float ImpactTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact Settings", meta = (AllowPrivateAccess = "true"))
	float DisintegrationTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact Settings", meta = (AllowPrivateAccess = "true"))
	float DisintegrationAmountPerHit = 0.020f;

	UStaticMesh* ImpactMesh = nullptr;
	TMap<uint32, UForceFieldImpact*> ActiveImpacts;
	UMaterialInterface* ForceFieldMaterial = nullptr;
	UMaterialInstanceDynamic* DynamicForceFieldMaterial = nullptr;
	UTimelineComponent* DisintegrationAnimationTimeline = nullptr;
	float CurrentDisintegrationAmount = 0.0f;

	friend class UForceFieldImpact;
};
