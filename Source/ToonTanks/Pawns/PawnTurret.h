#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "PawnTurret.generated.h"

UCLASS()
class TOONTANKS_API APawnTurret : public APawnBase {
	GENERATED_BODY()

public:
	APawnTurret();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	void CheckFireCondition();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float FireRate = 2.0f;

	FTimerHandle FireRateTimerHandle;
};
