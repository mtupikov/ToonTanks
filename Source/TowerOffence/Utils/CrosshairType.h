#pragma once

#include "UObject/Class.h"

UENUM()
enum class CrosshairType {
	None    UMETA(DisplayName = "None"),
	Bullet  UMETA(DisplayName = "Bullet"),
	Rocket  UMETA(DisplayName = "Rocket"),
	Grenade UMETA(DisplayName = "Grenade")
};
