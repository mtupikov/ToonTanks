#include "HUDBase.h"

#include "Engine/Canvas.h"

namespace {

const auto CrosshairSize = 64;

} // end anonymous namespace

AHUDBase::AHUDBase() {
	PrimaryActorTick.bCanEverTick = false;
}

void AHUDBase::SetPlayerSpeed(float Speed) {
	if (PlayerSpeed != Speed) {
		PlayerSpeed = Speed;
	}
}

void AHUDBase::SetCrosshairTexture(UTexture* Texture) {
	if (CrosshairTexture != Texture) {
		CrosshairTexture = Texture;
	}
}

void AHUDBase::DrawHUD() {
	Super::DrawHUD();

	const auto SizeX = Canvas->SizeX;
	const auto SizeY = Canvas->SizeY;
	const float CenterX = (SizeX / 2) - 8;
	const float CenterY = (SizeY / 2) - 8;

	CrosshairTexture = Cast<UTexture>(StaticLoadObject(UTexture::StaticClass(), nullptr, TEXT("Texture2D'/Game/Assets/Textures/T_Crosshair.T_Crosshair'")));
	if (!CrosshairTexture) {
		UE_LOG(LogTemp, Warning, TEXT("Cannot load crosshair texture"));
		return;
	}

	DrawTexture(
		CrosshairTexture,
		CenterX - CrosshairSize / 2,
		CenterY - CrosshairSize / 2,
		CrosshairSize,
		CrosshairSize,
		0,
		0,
		1,
		1,
		FLinearColor(0.0f, 0.0f, 0.0f, 0.5f),
		EBlendMode::BLEND_Translucent
	);
}
