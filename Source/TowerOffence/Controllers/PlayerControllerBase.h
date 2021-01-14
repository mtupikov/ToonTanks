#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"

UCLASS()
class TOWEROFFENCE_API APlayerControllerBase : public APlayerController {
	GENERATED_BODY()

public:
	void SetPlayerEnabledState(bool PlayerEnabled);
};
