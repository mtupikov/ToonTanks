#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "CrosshairType.h"

#include "WeaponSpreadManager.generated.h"

UCLASS()
class TOWEROFFENCE_API UWeaponSpreadManager : public UObject, public FTickableGameObject {
	GENERATED_BODY()

public:
	UWeaponSpreadManager();

	void SetPlayerSpeed(float Speed);
	void SetMaxShots(float Shots);
	void SetSpreadDecreaseValue(float Value);
	void SetSpreadRadiusCoef(float Value);
	void OnShotFired();

	float GetSpreadRadius() const;

	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

private:
	float PlayerSpeed = 0.0f;
	float ShotCounter = 0.0f;
	float MaxShots = 0.0f;
	float SpreadDecreaseValue = 0.0f;
	float SpreadRadiusCoef = 1.0f;
};
