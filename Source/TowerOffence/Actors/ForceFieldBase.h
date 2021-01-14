#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ForceFieldBase.generated.h"

class USphereComponent;
class UCurveFloat;
class UTimelineComponent;

UCLASS()
class TOWEROFFENCE_API AForceFieldBase : public AActor {
	GENERATED_BODY()

public:
	AForceFieldBase();

protected:
	virtual void BeginPlay() override;

private:
	virtual void Tick(float DeltaTime) override;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UTimelineComponent* ImpactAnimationTimeline = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* ForceFieldCollision = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ForceFieldMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact Settings", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* FloatCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact Settings", meta = (AllowPrivateAccess = "true"))
	float ImpactRadius = 100.0;

	UMaterialInterface* ForceFieldMaterial = nullptr;
	UMaterialInstanceDynamic* DynamicMaterial = nullptr;
};
