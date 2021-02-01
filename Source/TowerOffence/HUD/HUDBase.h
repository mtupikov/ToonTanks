#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Templates/SharedPointer.h"
#include "HUDBase.generated.h"

class APawnBase;
class UCrosshairManager;
class UUserWidget;
struct FBulletCrosshair;
struct FMissleCrosshair;
struct FGrenadeCrosshair;
struct FRailShotCrosshair;

UCLASS()
class TOWEROFFENCE_API AHUDBase : public AHUD {
	GENERATED_BODY()

public:
	AHUDBase();

	UCrosshairManager* GetCrosshairManager() const;

	void EnemyDamaged();
	void SetCrosshairCharge(float Value);

protected:
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

	void InitHealthBarWidget();
	void InitActionsPanelWidget();

	void DrawCrosshair();
	void DrawBulletCrosshair(const TSharedPtr<FBulletCrosshair>& Crosshair);
	void DrawMissleCrosshair(const TSharedPtr<FMissleCrosshair>& Crosshair);
	void DrawGrenadeCrosshair(const TSharedPtr<FGrenadeCrosshair>& Crosshair);
	void DrawRailShotCrosshair(const TSharedPtr<FRailShotCrosshair>& Crosshair);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> HealthClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actions Panel", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> PanelClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crosshair", meta = (AllowPrivateAccess = "true"))
	UCrosshairManager* CrosshairManager = nullptr;

	UUserWidget* HealthWidget = nullptr;
	UUserWidget* PanelWidget = nullptr;
	APawnBase* PlayerPawn = nullptr;
	FTimerHandle EnemyDamagedTimerHandle;
	float CrosshairCharge = 0.0f;
};
