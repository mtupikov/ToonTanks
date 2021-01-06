#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "PawnWithProjectileBase.generated.h"

class AProjectileBase;

UCLASS()
class TOONTANKS_API APawnWithProjectileBase : public APawnBase {
	GENERATED_BODY()

protected:
	virtual void Fire() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> ProjectileClass;
};
