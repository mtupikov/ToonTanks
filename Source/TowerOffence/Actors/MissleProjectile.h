#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "MissleProjectile.generated.h"

UCLASS()
class TOWEROFFENCE_API AMissleProjectile : public AProjectileBase {
	GENERATED_BODY()

	virtual void Detonate() override;
};
