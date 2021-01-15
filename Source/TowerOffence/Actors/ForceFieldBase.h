#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ForceFieldBase.generated.h"

class USphereComponent;
class UCurveFloat;
class UTimelineComponent;
class UForceFieldImpact;

UCLASS()
class TOWEROFFENCE_API AForceFieldBase : public AActor {
	GENERATED_BODY()

public:
	AForceFieldBase();

private:
	virtual void Tick(float DeltaTime) override;

	void RemoveFinishedImpact(uint32 Key);
	
	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* ForceFieldCollision = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ForceFieldMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact Settings", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* FloatCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact Settings", meta = (AllowPrivateAccess = "true"))
	float ImpactRadius = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impact Settings", meta = (AllowPrivateAccess = "true"))
	float ImpactTime = 0.5f;

	UStaticMesh* ImpactMesh = nullptr;
	TMap<uint32, UForceFieldImpact*> ActiveImpacts;

	friend class UForceFieldImpact;
};
