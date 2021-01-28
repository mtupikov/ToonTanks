#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDBase.generated.h"

class APawnBase;
class UCrosshairManager;
class UUserWidget;

UCLASS()
class TOWEROFFENCE_API AHUDBase : public AHUD {
	GENERATED_BODY()

public:
	AHUDBase();

	UCrosshairManager* GetCrosshairManager() const;

protected:
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

	void InitHealthBarWidget();
	void InitActionsPanelWidget();

	void DrawCrosshair();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> HealthClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions Panel", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> PanelClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	UCrosshairManager* CrosshairManager = nullptr;

	UUserWidget* HealthWidget = nullptr;
	UUserWidget* PanelWidget = nullptr;
	APawnBase* PlayerPawn = nullptr;
};
