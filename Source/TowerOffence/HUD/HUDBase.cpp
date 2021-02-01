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

void AHUDBase::SetCrosshairCharge(float Value) {
	if (CrosshairCharge != Value) {
		CrosshairCharge = Value;
	}
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

	switch (CurrentCrosshairType) {
	case CrosshairType::Bullet: {
		DrawBulletCrosshair(StaticCastSharedPtr<FBulletCrosshair>(Crosshair));
		break;
	}
	case CrosshairType::Rocket: {
		DrawMissleCrosshair(StaticCastSharedPtr<FMissleCrosshair>(Crosshair));
		break;
	}
	case CrosshairType::Grenade: {
		DrawGrenadeCrosshair(StaticCastSharedPtr<FGrenadeCrosshair>(Crosshair));
		break;
	}
	case CrosshairType::RailShot: {
		DrawRailShotCrosshair(StaticCastSharedPtr<FRailShotCrosshair>(Crosshair));
		break;
	}
	}
}

void AHUDBase::DrawBulletCrosshair(const TSharedPtr<FBulletCrosshair>& Crosshair) {
	FLinearColor CrosshairColor;
	const auto SizeX = Canvas->SizeX;
	const auto SizeY = Canvas->SizeY;
	const float CenterX = SizeX / 2;
	const float CenterY = SizeY / 2;
	const auto ScreenX = CenterX - Crosshair->Size / 2;
	const auto ScreenY = CenterY - Crosshair->Size / 2;
	auto Offset = PlayerPawn->GetShootComponent()->GetFireSpreadRadius();
	const auto Alpha = 0.55f - FMath::GetMappedRangeValueClamped({ 0.0f, 10.0f }, { 0.05f, 0.5f }, Offset);

	if (GetWorld()->GetTimerManager().IsTimerActive(EnemyDamagedTimerHandle)) {
		Offset += 3.0f;
		CrosshairColor = FLinearColor(1.0f, 0.0f, 0.0f, Alpha);
	} else {
		CrosshairColor = FLinearColor(0.0f, 0.0f, 0.0f, Alpha);
	}

	if (Crosshair->CenterTexture) {
		DrawTexture(
			Crosshair->CenterTexture,
			ScreenX,
			ScreenY,
			Crosshair->Size,
			Crosshair->Size,
			0,
			0,
			1,
			1,
			FLinearColor(0.0f, 0.0f, 0.0f, 0.5f),
			EBlendMode::BLEND_Translucent
		);
	}

	if (Crosshair->TopLeftTexture) {
		DrawTexture(
			Crosshair->TopLeftTexture,
			ScreenX - Offset,
			ScreenY - Offset,
			Crosshair->Size,
			Crosshair->Size,
			0,
			0,
			1,
			1,
			CrosshairColor,
			EBlendMode::BLEND_Translucent
		);
	}

	if (Crosshair->TopRightTexture) {
		DrawTexture(
			Crosshair->TopRightTexture,
			ScreenX + Offset,
			ScreenY - Offset,
			Crosshair->Size,
			Crosshair->Size,
			0,
			0,
			1,
			1,
			CrosshairColor,
			EBlendMode::BLEND_Translucent
		);
	}

	if (Crosshair->BottomLeftTexture) {
		DrawTexture(
			Crosshair->BottomLeftTexture,
			ScreenX - Offset,
			ScreenY + Offset,
			Crosshair->Size,
			Crosshair->Size,
			0,
			0,
			1,
			1,
			CrosshairColor,
			EBlendMode::BLEND_Translucent
		);
	}

	if (Crosshair->BottomRightTexture) {
		DrawTexture(
			Crosshair->BottomRightTexture,
			ScreenX + Offset,
			ScreenY + Offset,
			Crosshair->Size,
			Crosshair->Size,
			0,
			0,
			1,
			1,
			CrosshairColor,
			EBlendMode::BLEND_Translucent
		);
	}
}

void AHUDBase::DrawMissleCrosshair(const TSharedPtr<FMissleCrosshair>& Crosshair) {
	FLinearColor CrosshairColor;
	const auto SizeX = Canvas->SizeX;
	const auto SizeY = Canvas->SizeY;
	const float CenterX = SizeX / 2;
	const float CenterY = SizeY / 2;
	const auto ScreenX = CenterX - Crosshair->Size / 2;
	const auto ScreenY = CenterY - Crosshair->Size / 2;
	auto Offset = PlayerPawn->GetShootComponent()->GetFireSpreadRadius();
	const auto Alpha = 0.55f - FMath::GetMappedRangeValueClamped({ 0.0f, 10.0f }, { 0.05f, 0.5f }, Offset);

	if (GetWorld()->GetTimerManager().IsTimerActive(EnemyDamagedTimerHandle)) {
		Offset += 3.0f;
		CrosshairColor = FLinearColor(1.0f, 0.0f, 0.0f, Alpha);
	} else {
		CrosshairColor = FLinearColor(0.0f, 0.0f, 0.0f, Alpha);
	}

	if (Crosshair->CenterTexture) {
		DrawTexture(
			Crosshair->CenterTexture,
			ScreenX,
			ScreenY,
			Crosshair->Size,
			Crosshair->Size,
			0,
			0,
			1,
			1,
			FLinearColor(0.0f, 0.0f, 0.0f, 0.5f),
			EBlendMode::BLEND_Translucent
		);
	}

	if (Crosshair->TopLeftTexture) {
		DrawTexture(
			Crosshair->TopLeftTexture,
			ScreenX - Offset,
			ScreenY - Offset,
			Crosshair->Size,
			Crosshair->Size,
			0,
			0,
			1,
			1,
			CrosshairColor,
			EBlendMode::BLEND_Translucent
		);
	}

	if (Crosshair->TopRightTexture) {
		DrawTexture(
			Crosshair->TopRightTexture,
			ScreenX + Offset,
			ScreenY - Offset,
			Crosshair->Size,
			Crosshair->Size,
			0,
			0,
			1,
			1,
			CrosshairColor,
			EBlendMode::BLEND_Translucent
		);
	}

	if (Crosshair->BottomTexture) {
		DrawTexture(
			Crosshair->BottomTexture,
			ScreenX,
			ScreenY + Offset,
			Crosshair->Size,
			Crosshair->Size,
			0,
			0,
			1,
			1,
			CrosshairColor,
			EBlendMode::BLEND_Translucent
		);
	}
}

void AHUDBase::DrawGrenadeCrosshair(const TSharedPtr<FGrenadeCrosshair>& Crosshair) {
	FLinearColor CrosshairColor;
	const auto SizeX = Canvas->SizeX;
	const auto SizeY = Canvas->SizeY;
	const float CenterX = SizeX / 2;
	const float CenterY = SizeY / 2;
	const auto ScreenX = CenterX - Crosshair->Size / 2;
	const auto ScreenY = CenterY - Crosshair->Size / 2;
	const auto Alpha = 0.55f;

	if (GetWorld()->GetTimerManager().IsTimerActive(EnemyDamagedTimerHandle)) {
		CrosshairColor = FLinearColor(1.0f, 0.0f, 0.0f, Alpha);
	} else {
		CrosshairColor = FLinearColor(0.0f, 0.0f, 0.0f, Alpha);
	}

	if (Crosshair->CenterTexture) {
		DrawTexture(
			Crosshair->CenterTexture,
			ScreenX,
			ScreenY,
			Crosshair->Size,
			Crosshair->Size,
			0,
			0,
			1,
			1,
			CrosshairColor,
			EBlendMode::BLEND_Translucent
		);
	}

	if (Crosshair->SliderTexture) {
		DrawTexture(
			Crosshair->SliderTexture,
			ScreenX,
			ScreenY - CrosshairCharge * (Crosshair->Size / 2.35f),
			Crosshair->Size,
			Crosshair->Size,
			0,
			0,
			1,
			1,
			FLinearColor(0.0f, 0.0f, 0.0f, 0.5f),
			EBlendMode::BLEND_Translucent
		);
	}
}

void AHUDBase::DrawRailShotCrosshair(const TSharedPtr<FRailShotCrosshair>& Crosshair) {
	FLinearColor CrosshairColor;
	const auto SizeX = Canvas->SizeX;
	const auto SizeY = Canvas->SizeY;
	const float CenterX = SizeX / 2;
	const float CenterY = SizeY / 2;
	const auto ScreenX = CenterX - Crosshair->Size / 2;
	const auto ScreenY = CenterY - Crosshair->Size / 2;
	const auto Alpha = 0.55f;
	const auto ChargeOffset = CrosshairCharge * 8.0f;

	if (GetWorld()->GetTimerManager().IsTimerActive(EnemyDamagedTimerHandle)) {
		CrosshairColor = FLinearColor(1.0f, 0.0f, 0.0f, Alpha);
	} else {
		CrosshairColor = FLinearColor(0.0f, 0.0f, 0.0f, Alpha);
	}

	if (Crosshair->TopLeftTexture) {
		DrawTexture(
			Crosshair->TopLeftTexture,
			ScreenX + ChargeOffset,
			ScreenY + ChargeOffset,
			Crosshair->Size,
			Crosshair->Size,
			0,
			0,
			1,
			1,
			CrosshairColor,
			EBlendMode::BLEND_Translucent
		);
	}

	if (Crosshair->TopRightTexture) {
		DrawTexture(
			Crosshair->TopRightTexture,
			ScreenX - ChargeOffset,
			ScreenY + ChargeOffset,
			Crosshair->Size,
			Crosshair->Size,
			0,
			0,
			1,
			1,
			CrosshairColor,
			EBlendMode::BLEND_Translucent
		);
	}

	if (Crosshair->BottomLeftTexture) {
		DrawTexture(
			Crosshair->BottomLeftTexture,
			ScreenX + ChargeOffset,
			ScreenY - ChargeOffset,
			Crosshair->Size,
			Crosshair->Size,
			0,
			0,
			1,
			1,
			CrosshairColor,
			EBlendMode::BLEND_Translucent
		);
	}

	if (Crosshair->BottomRightTexture) {
		DrawTexture(
			Crosshair->BottomRightTexture,
			ScreenX - ChargeOffset,
			ScreenY - ChargeOffset,
			Crosshair->Size,
			Crosshair->Size,
			0,
			0,
			1,
			1,
			CrosshairColor,
			EBlendMode::BLEND_Translucent
		);
	}
}
