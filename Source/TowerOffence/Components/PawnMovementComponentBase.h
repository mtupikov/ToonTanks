#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "PawnMovementComponentBase.generated.h"

UCLASS()
class TOWEROFFENCE_API UPawnMovementComponentBase : public UPawnMovementComponent {
	GENERATED_BODY()

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetMoveSpeed(float Value);

private:
	float MoveSpeed = 0.0f;
};
