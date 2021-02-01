#pragma once

class UTexture2D;

struct FCrosshairBase {
	float Size = 32.0f;
};

struct FBulletCrosshair : public FCrosshairBase {
	UTexture2D* CenterTexture = nullptr;
	UTexture2D* TopLeftTexture = nullptr;
	UTexture2D* TopRightTexture = nullptr;
	UTexture2D* BottomLeftTexture = nullptr;
	UTexture2D* BottomRightTexture = nullptr;
};

struct FMissleCrosshair : public FCrosshairBase {
	UTexture2D* CenterTexture = nullptr;
	UTexture2D* TopLeftTexture = nullptr;
	UTexture2D* TopRightTexture = nullptr;
	UTexture2D* BottomTexture = nullptr;
};

struct FGrenadeCrosshair : public FCrosshairBase {
	UTexture2D* CenterTexture = nullptr;
	UTexture2D* SliderTexture = nullptr;
};

struct FRailShotCrosshair : public FCrosshairBase {
	UTexture2D* TopLeftTexture = nullptr;
	UTexture2D* TopRightTexture = nullptr;
	UTexture2D* BottomLeftTexture = nullptr;
	UTexture2D* BottomRightTexture = nullptr;
};
