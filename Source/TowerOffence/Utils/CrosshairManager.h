#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Templates/SharedPointer.h"

#include "Crosshair.h"
#include "CrosshairType.h"

#include "CrosshairManager.generated.h"

UCLASS()
class TOWEROFFENCE_API UCrosshairManager : public UObject {
	GENERATED_BODY()

public:
	UCrosshairManager();

	const TSharedPtr<FCrosshairBase>& GetCrosshair(CrosshairType Type);
	CrosshairType GetCurrentCrosshairType();
	void SetCrosshairType(CrosshairType Type);

private:
	TMap<CrosshairType, TSharedPtr<FCrosshairBase>> Crosshairs;
	CrosshairType CurrentCrosshairType;
};
