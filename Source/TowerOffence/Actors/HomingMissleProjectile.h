#pragma once

#include "CoreMinimal.h"
#include "MissleProjectile.h"
#include "HomingMissleProjectile.generated.h"

UCLASS()
class TOWEROFFENCE_API AHomingMissleProjectile : public AMissleProjectile {
	GENERATED_BODY()

public:
	AHomingMissleProjectile();

	void SetHomingTarget(USceneComponent* Target);
};
