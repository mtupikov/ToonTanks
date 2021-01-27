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

	void Init(bool ActiveOnSpawn);

	void Activate();
	void Deactivate();

	void CreateImpact(const FVector& ImpactPoint);
	bool IsInside(const FVector& Point) const;

	bool IsActive() const;

protected:
	virtual void BeginPlay() override;

private:
	virtual void Tick(float DeltaTime) override;

	void RemoveFinishedImpact(uint32 Key);
	void CreateDynamicForceFieldMaterial();
	void InitActivationTimeline();
	void InitDisintegrationTimeline();

	UFUNCTION()
	void OnHealthChanged(float Health);

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
	void ActivationTimelineCallback(float Value);

	UFUNCTION()
	void DisintegrationTimelineCallback(float Value);

	UFUNCTION()
	void DisintegrationTimelineFinishedCallback();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* ForceFieldCollision = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ForceFieldMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activation", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* ActivationFloatCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activation", meta = (AllowPrivateAccess = "true"))
	float ActivationTime = 1.0f; // when modifying - do not forget to change ActivationFloatCurve

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact Settings", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* ImpactFloatCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact Settings", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* DisintegrationFloatCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact Settings", meta = (AllowPrivateAccess = "true"))
	float ImpactRadius = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact Settings", meta = (AllowPrivateAccess = "true"))
	float ImpactTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact Settings", meta = (AllowPrivateAccess = "true"))
	float DisintegrationTime = 1.0f; // when modifying - do not forget to change DisintegrationFloatCurve

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact Settings", meta = (AllowPrivateAccess = "true"))
	float DisintegrationAmountPerHit = 0.1f;

	UStaticMesh* ImpactMesh = nullptr;
	TMap<uint32, UForceFieldImpact*> ActiveImpacts;
	UMaterialInterface* ForceFieldMaterial = nullptr;
	UMaterialInstanceDynamic* DynamicForceFieldMaterial = nullptr;
	UTimelineComponent* DisintegrationAnimationTimeline = nullptr;
	UTimelineComponent* ActivationAnimationTimeline = nullptr;
	float CurrentDisintegrationAmount = 0.0f;
	float RelativeDisintegrationAmountMax = 0.5f;
	float RealDisintegrationAmountMax = 1.0f;

	friend class UForceFieldImpact;
};
