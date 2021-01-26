#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDBase.generated.h"

class APawnBase;
class UCrosshairManager;

UCLASS()
class TOWEROFFENCE_API AHUDBase : public AHUD {
	GENERATED_BODY()

public:
	AHUDBase();

	UCrosshairManager* GetCrosshairManager() const;

protected:
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	UCrosshairManager* CrosshairManager = nullptr;

	APawnBase* PlayerPawn = nullptr;
};
