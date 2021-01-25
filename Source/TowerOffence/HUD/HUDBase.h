#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDBase.generated.h"

UCLASS()
class TOWEROFFENCE_API AHUDBase : public AHUD {
	GENERATED_BODY()

public:
	AHUDBase();

	void SetPlayerSpeed(float Speed);
	void SetCrosshairTexture(UTexture* Texture);

protected:
	virtual void DrawHUD() override;

private:
	UTexture* CrosshairTexture = nullptr;
	float PlayerSpeed;
};
