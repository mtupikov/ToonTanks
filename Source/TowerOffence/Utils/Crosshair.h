#pragma once

class UTexture2D;

struct FCrosshair {
	UTexture2D* CenterTexture = nullptr;
	UTexture2D* TopLeftTexture = nullptr;
	UTexture2D* TopRightTexture = nullptr;
	UTexture2D* BottomLeftTexture = nullptr;
	UTexture2D* BottomRightTexture = nullptr;

	float Size = 32.0f;
};
