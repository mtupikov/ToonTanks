#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "PawnTurret.generated.h"

class APawnTank;

UCLASS()
class TOONTANKS_API APawnTurret : public APawnBase {
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
	float ReturnDistanceToPlayer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float FireRate = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float FireRange = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float MaximumLeftRelativeRotation = APawnBase::MaximumRotationAngle(); // must be 0 <= x <= 180

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float MaximumRightRelativeRotation = APawnBase::MaximumRotationAngle(); // must be 0 <= x <= 180

	FTimerHandle FireRateTimerHandle;
	APawnTank* PlayerPawn = nullptr;
};
