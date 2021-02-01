#include "CrosshairManager.h"

namespace {

TMap<CrosshairType, TSharedPtr<FCrosshairBase>> CreateCrosshairs() {
	const auto CreateTextureFromPath = [](const TCHAR* Name) {
		return Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, Name));
	};

	auto BulletCrosshair = TSharedPtr<FBulletCrosshair>(new FBulletCrosshair);
	BulletCrosshair->CenterTexture = CreateTextureFromPath(TEXT("Texture2D'/Game/Assets/Textures/HUD/T_BulletCenter.T_BulletCenter'"));
	BulletCrosshair->TopLeftTexture = CreateTextureFromPath(TEXT("Texture2D'/Game/Assets/Textures/HUD/T_BulletTopLeft.T_BulletTopLeft'"));
	BulletCrosshair->TopRightTexture = CreateTextureFromPath(TEXT("Texture2D'/Game/Assets/Textures/HUD/T_BulletTopRight.T_BulletTopRight'"));
	BulletCrosshair->BottomLeftTexture = CreateTextureFromPath(TEXT("Texture2D'/Game/Assets/Textures/HUD/T_BulletBottomLeft.T_BulletBottomLeft'"));
	BulletCrosshair->BottomRightTexture = CreateTextureFromPath(TEXT("Texture2D'/Game/Assets/Textures/HUD/T_BulletBottomRight.T_BulletBottomRight'"));
	BulletCrosshair->Size = 40;

	auto MissleCrosshair = TSharedPtr<FMissleCrosshair>(new FMissleCrosshair);
	MissleCrosshair->CenterTexture = CreateTextureFromPath(TEXT("Texture2D'/Game/Assets/Textures/HUD/T_MissleCenter.T_MissleCenter'"));
	MissleCrosshair->TopLeftTexture = CreateTextureFromPath(TEXT("Texture2D'/Game/Assets/Textures/HUD/T_MissleTopLeft.T_MissleTopLeft'"));
	MissleCrosshair->TopRightTexture = CreateTextureFromPath(TEXT("Texture2D'/Game/Assets/Textures/HUD/T_MissleTopRight.T_MissleTopRight'"));
	MissleCrosshair->BottomTexture = CreateTextureFromPath(TEXT("Texture2D'/Game/Assets/Textures/HUD/T_MissleBottom.T_MissleBottom'"));
	MissleCrosshair->Size = 40;

	auto GrenadeCrosshair = TSharedPtr<FGrenadeCrosshair>(new FGrenadeCrosshair);
	GrenadeCrosshair->CenterTexture = CreateTextureFromPath(TEXT("Texture2D'/Game/Assets/Textures/HUD/T_GrenadeCrosshair.T_GrenadeCrosshair'"));
	GrenadeCrosshair->SliderTexture = CreateTextureFromPath(TEXT("Texture2D'/Game/Assets/Textures/HUD/T_GrenadePowerSlider.T_GrenadePowerSlider'"));
	GrenadeCrosshair->Size = 60;

	const TMap<CrosshairType, TSharedPtr<FCrosshairBase>> Map {
		{ CrosshairType::Bullet, BulletCrosshair },
		{ CrosshairType::Rocket, MissleCrosshair },
		{ CrosshairType::Grenade, GrenadeCrosshair }
	};

	return Map;
}

} // end anonymous namespace

UCrosshairManager::UCrosshairManager() {
	Crosshairs = CreateCrosshairs();
}

const TSharedPtr<FCrosshairBase>& UCrosshairManager::GetCrosshair(CrosshairType Type) {
	ensure(Crosshairs.Contains(Type));
	return Crosshairs[Type];
}

CrosshairType UCrosshairManager::GetCurrentCrosshairType() {
	return CurrentCrosshairType;
}

void UCrosshairManager::SetCrosshairType(CrosshairType Type) {
	if (CurrentCrosshairType != Type) {
		CurrentCrosshairType = Type;
	}
}
