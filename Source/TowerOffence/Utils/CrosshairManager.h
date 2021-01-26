#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Crosshair.h"
#include "CrosshairType.h"

#include "CrosshairManager.generated.h"

UCLASS()
class TOWEROFFENCE_API UCrosshairManager : public UObject {
	GENERATED_BODY()

public:
	UCrosshairManager();

	const FCrosshair& GetCrosshair(CrosshairType Type);
	CrosshairType GetCurrentCrosshairType();
	void SetCrosshairType(CrosshairType Type);

private:
	TMap<CrosshairType, FCrosshair> Crosshairs;
	CrosshairType CurrentCrosshairType;
};
