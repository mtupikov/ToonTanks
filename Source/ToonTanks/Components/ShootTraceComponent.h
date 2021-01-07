#pragma once

#include "CoreMinimal.h"
#include "ShootComponent.h"
#include "ShootTraceComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOONTANKS_API UShootTraceComponent : public UShootComponent {
	GENERATED_BODY()

public:
	virtual void Fire(const FVector& SpawnLocation, const FRotator& SpawnRotation, AActor* Owner, USceneComponent* Target = nullptr) override;
};
