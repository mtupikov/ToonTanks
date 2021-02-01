#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Explosion.generated.h"

class URadialForceComponent;

UCLASS()
class TOWEROFFENCE_API AExplosion : public AActor {
	GENERATED_BODY()

public:
	AExplosion();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	URadialForceComponent* RadialForceComponent = nullptr;
};
