#include "CrosshairManager.h"

namespace {

TMap<CrosshairType, FCrosshair> CreateCrosshairs() {
	FCrosshair BulletCrosshair;
	BulletCrosshair.CenterTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("Texture2D'/Game/Assets/Textures/HUD/T_CrosshairCenter.T_CrosshairCenter'")));
	BulletCrosshair.TopLeftTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("Texture2D'/Game/Assets/Textures/HUD/T_CrosshairTopLeft.T_CrosshairTopLeft'")));
	BulletCrosshair.TopRightTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("Texture2D'/Game/Assets/Textures/HUD/T_CrosshairTopRight.T_CrosshairTopRight'")));
	BulletCrosshair.BottomLeftTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("Texture2D'/Game/Assets/Textures/HUD/T_CrosshairBottomLeft.T_CrosshairBottomLeft'")));
	BulletCrosshair.BottomRightTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("Texture2D'/Game/Assets/Textures/HUD/T_CrosshairBottomRight.T_CrosshairBottomRight'")));
	BulletCrosshair.Size = 40;

	const TMap<CrosshairType, FCrosshair> Map {
		{ CrosshairType::Bullet, BulletCrosshair }
	};

	return Map;
}

} // end anonymous namespace

UCrosshairManager::UCrosshairManager() {
	Crosshairs = CreateCrosshairs();
}

const FCrosshair& UCrosshairManager::GetCrosshair(CrosshairType Type) {
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
