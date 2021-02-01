#include "CrosshairManager.h"

namespace {

TMap<CrosshairType, TSharedPtr<FCrosshairBase>> CreateCrosshairs() {
	const auto CreateTextureFromPath = [](const TCHAR* Name) {
		return Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, Name));
	};

	auto BulletCrosshair = TSharedPtr<FBulletCrosshair>(new FBulletCrosshair);
	BulletCrosshair->CenterTexture = CreateTextureFromPath(TEXT("Texture2D'/Game/Assets/Textures/HUD/T_CrosshairCenter.T_CrosshairCenter'"));
	BulletCrosshair->TopLeftTexture = CreateTextureFromPath(TEXT("Texture2D'/Game/Assets/Textures/HUD/T_CrosshairTopLeft.T_CrosshairTopLeft'"));
	BulletCrosshair->TopRightTexture = CreateTextureFromPath(TEXT("Texture2D'/Game/Assets/Textures/HUD/T_CrosshairTopRight.T_CrosshairTopRight'"));
	BulletCrosshair->BottomLeftTexture = CreateTextureFromPath(TEXT("Texture2D'/Game/Assets/Textures/HUD/T_CrosshairBottomLeft.T_CrosshairBottomLeft'"));
	BulletCrosshair->BottomRightTexture = CreateTextureFromPath(TEXT("Texture2D'/Game/Assets/Textures/HUD/T_CrosshairBottomRight.T_CrosshairBottomRight'"));
	BulletCrosshair->Size = 40;

	auto GrenadeCrosshair = TSharedPtr<FGrenadeCrosshair>(new FGrenadeCrosshair);
	GrenadeCrosshair->CenterTexture = CreateTextureFromPath(TEXT("Texture2D'/Game/Assets/Textures/HUD/T_GrenadeCrosshair.T_GrenadeCrosshair'"));
	GrenadeCrosshair->SliderTexture = CreateTextureFromPath(TEXT("Texture2D'/Game/Assets/Textures/HUD/T_GrenadePowerSlider.T_GrenadePowerSlider'"));
	GrenadeCrosshair->Size = 60;

	const TMap<CrosshairType, TSharedPtr<FCrosshairBase>> Map {
		{ CrosshairType::Bullet, BulletCrosshair },
		{ CrosshairType::Rocket, BulletCrosshair },
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
