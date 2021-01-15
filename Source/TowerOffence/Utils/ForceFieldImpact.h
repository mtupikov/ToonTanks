#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ForceFieldImpact.generated.h"

class AForceFieldBase;
class UCurveFloat;
class UMaterialInstanceDynamic;
class UTimelineComponent;

UCLASS()
class TOWEROFFENCE_API UForceFieldImpact : public UObject {
	GENERATED_BODY()

public:
	void Init(
		AForceFieldBase* Owner,
		const FVector& ImpactPoint
	);

	UFUNCTION()
	void TimelineCallback(float Value);

	UFUNCTION()
	void TimelineFinishedCallback();

	void Tick(float DeltaTime);

private:
	AForceFieldBase* Owner = nullptr;
	UTimelineComponent* ImpactAnimationTimeline = nullptr;
	UMaterialInstanceDynamic* DynamicMaterial = nullptr;
	UStaticMeshComponent* ImpactSphere = nullptr;
};
