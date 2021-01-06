#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "PawnWithTraceBase.generated.h"

UCLASS()
class TOONTANKS_API APawnWithTraceBase : public APawnBase {
	GENERATED_BODY()

protected:
	virtual void Fire() override;
};
