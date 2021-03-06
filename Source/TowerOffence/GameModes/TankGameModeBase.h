#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TankGameModeBase.generated.h"

class APawnBase;
class APlayerControllerBase;

UCLASS()
class TOWEROFFENCE_API ATankGameModeBase : public AGameModeBase {
	GENERATED_BODY()

public:
	ATankGameModeBase();

	void ActorDied(AActor* DeadActor);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void GameStart();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool PlayerWon);

private:
	void HandleGameStart();
	void HandleGameOver(bool PlayerWon);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Loop", meta = (AllowPrivateAccess = "true"))
	int32 StartDelay = 3;

	APlayerControllerBase* PlayerController = nullptr;
	APawnBase* PlayerTank = nullptr;
	uint32 TargetTurretsCount = 0;
};
