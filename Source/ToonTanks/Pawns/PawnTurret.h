#pragma once

#include "CoreMinimal.h"
#include "PawnWithProjectileBase.h"
#include "PawnTurret.generated.h"

class ATankWithProjectile;

UCLASS()
class TOONTANKS_API APawnTurret : public APawnWithProjectileBase {
	GENERATED_BODY()

public:
	APawnTurret();

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

	FTimerHandle FireRateTimerHandle;
	APawnBase* PlayerPawn = nullptr;
	bool bIsIdleRotatingLeft = true;
};
