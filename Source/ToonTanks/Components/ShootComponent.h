#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShootComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOONTANKS_API UShootComponent : public UActorComponent {
	GENERATED_BODY()

public:
	virtual void Fire(const FVector& SpawnLocation, const FRotator& SpawnRotation, AActor* Owner);
};
