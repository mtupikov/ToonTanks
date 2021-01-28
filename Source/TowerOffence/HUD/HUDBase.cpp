#include "HUDBase.h"

#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"
#include "Kismet/GameplayStatics.h"

#include "TowerOffence/Pawns/PawnBase.h"
#include "TowerOffence/Components/ShootAmmunitionComponent.h"
#include "TowerOffence/Utils/CrosshairManager.h"

AHUDBase::AHUDBase() {
	PrimaryActorTick.bCanEverTick = false;
}

UCrosshairManager* AHUDBase::GetCrosshairManager() const {
	return CrosshairManager;
}

void AHUDBase::EnemyDamaged() {
	GetWorld()->GetTimerManager().SetTimer(EnemyDamagedTimerHandle, 0.3f, false);
}

void AHUDBase::BeginPlay() {
	PlayerPawn = Cast<APawnBase>(UGameplayStatics::GetPlayerPawn(this, 0));
	CrosshairManager = NewObject<UCrosshairManager>(UCrosshairManager::StaticClass(), TEXT("Crosshair manager"));

	InitHealthBarWidget();
	InitActionsPanelWidget();
}

void AHUDBase::DrawHUD() {
	Super::DrawHUD();

	if (PlayerPawn->IsAlive()) {
		DrawCrosshair();
	}
}

void AHUDBase::InitHealthBarWidget() {
	if (HealthClass) {
		if (auto* Widget = CreateWidget<UUserWidget>(GetWorld(), HealthClass)) {
			HealthWidget = Widget;
			HealthWidget->AddToViewport();
		}
	}
}

void AHUDBase::InitActionsPanelWidget() {
	if (PanelClass) {
		if (auto* Widget = CreateWidget<UUserWidget>(GetWorld(), PanelClass)) {
			PanelWidget = Widget;
			PanelWidget->AddToViewport();
		}
	}
}

void AHUDBase::DrawCrosshair() {
	const auto CurrentCrosshairType = CrosshairManager->GetCurrentCrosshairType();
	const auto& Crosshair = CrosshairManager->GetCrosshair(CurrentCrosshairType);

	FLinearColor CrosshairColor;
	const auto SizeX = Canvas->SizeX;
	const auto SizeY = Canvas->SizeY;
	const float CenterX = SizeX / 2;
	const float CenterY = SizeY / 2;
	auto Offset = PlayerPawn->GetShootComponent()->GetFireSpreadRadius();
	const auto Alpha = 0.55f - FMath::GetMappedRangeValueClamped({ 0.0f, 10.0f }, { 0.05f, 0.5f }, Offset);

	if (GetWorld()->GetTimerManager().IsTimerActive(EnemyDamagedTimerHandle)) {
		Offset += 3.0f;
		CrosshairColor = FLinearColor(1.0f, 0.0f, 0.0f, Alpha);
	} else {
		CrosshairColor = FLinearColor(0.0f, 0.0f, 0.0f, Alpha);
	}

	if (Crosshair.CenterTexture) {
		DrawTexture(
			Crosshair.CenterTexture,
			CenterX - Crosshair.Size / 2,
			CenterY - Crosshair.Size / 2,
			Crosshair.Size,
			Crosshair.Size,
			0,
			0,
			1,
			1,
			FLinearColor(0.0f, 0.0f, 0.0f, 0.5f),
			EBlendMode::BLEND_Translucent
		);
	}

	if (Crosshair.TopLeftTexture) {
		DrawTexture(
			Crosshair.TopLeftTexture,
			CenterX - Crosshair.Size / 2 - Offset,
			CenterY - Crosshair.Size / 2 - Offset,
			Crosshair.Size,
			Crosshair.Size,
			0,
			0,
			1,
			1,
			CrosshairColor,
			EBlendMode::BLEND_Translucent
		);
	}

	if (Crosshair.TopRightTexture) {
		DrawTexture(
			Crosshair.TopRightTexture,
			CenterX - Crosshair.Size / 2 + Offset,
			CenterY - Crosshair.Size / 2 - Offset,
			Crosshair.Size,
			Crosshair.Size,
			0,
			0,
			1,
			1,
			CrosshairColor,
			EBlendMode::BLEND_Translucent
		);
	}

	if (Crosshair.BottomLeftTexture) {
		DrawTexture(
			Crosshair.BottomLeftTexture,
			CenterX - Crosshair.Size / 2 - Offset,
			CenterY - Crosshair.Size / 2 + Offset,
			Crosshair.Size,
			Crosshair.Size,
			0,
			0,
			1,
			1,
			CrosshairColor,
			EBlendMode::BLEND_Translucent
		);
	}

	if (Crosshair.BottomRightTexture) {
		DrawTexture(
			Crosshair.BottomRightTexture,
			CenterX - Crosshair.Size / 2 + Offset,
			CenterY - Crosshair.Size / 2 + Offset,
			Crosshair.Size,
			Crosshair.Size,
			0,
			0,
			1,
			1,
			CrosshairColor,
			EBlendMode::BLEND_Translucent
		);
	}
}
