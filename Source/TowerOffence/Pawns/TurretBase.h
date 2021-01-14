#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "TurretBase.generated.h"

class ATankBase;

UCLASS()
class TOWEROFFENCE_API ATurretBase : public APawnBase {
	GENERATED_BODY()

public:
	ATurretBase();

	virtual void Tick(float DeltaTime) override;
	virtual void HandleDestruction() override;

protected:
	virtual void BeginPlay() override;

private:
	void CheckFireCondition();
	void SelectPlayerPawn();
	void RotateIdle();
	float ReturnDistanceToPlayer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float FireRange = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float MaximumLeftRelativeRotation = APawnBase::MaximumRotationAngle();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float MaximumRightRelativeRotation = APawnBase::MaximumRotationAngle();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float IdleRotationSpeed = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bHasHomingProjectile = false;

	FTimerHandle FireRateTimerHandle;
	ATankBase* PlayerPawn = nullptr;
	bool bIsIdleRotatingLeft = true;
};
