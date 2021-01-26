#include "WeaponSpreadManager.h"

void UWeaponSpreadManager::SetPlayerSpeed(float Speed) {
	if (PlayerSpeed != Speed) {
		PlayerSpeed = Speed;
	}
}

void UWeaponSpreadManager::SetMaxShots(float Shots) {
	if (MaxShots != Shots) {
		MaxShots = Shots;
	}
}

void UWeaponSpreadManager::SetSpreadDecreaseValue(float Value) {
	if (SpreadDecreaseValue != Value) {
		SpreadDecreaseValue = Value;
	}
}

void UWeaponSpreadManager::OnShotFired() {
	ShotCounter += 1.0f;
	ShotCounter = FMath::Clamp(ShotCounter, 0.0f, MaxShots);
}

float UWeaponSpreadManager::GetSpreadRadius() const {
	return ShotCounter * 2.0f;
}

void UWeaponSpreadManager::Tick(float DeltaTime) {
	if (FMath::IsNearlyZero(ShotCounter)) {
		return;
	}

	ShotCounter -= SpreadDecreaseValue;
	ShotCounter = FMath::Clamp(ShotCounter, 0.0f, MaxShots);
}

TStatId UWeaponSpreadManager::GetStatId() const {
	return UObject::GetStatID();
}

UWeaponSpreadManager::UWeaponSpreadManager() {}
