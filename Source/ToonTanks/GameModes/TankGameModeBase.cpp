#include "TankGameModeBase.h"

ATankGameModeBase::ATankGameModeBase() {}

void ATankGameModeBase::ActorDied(AActor* DeadActor) {
	UE_LOG(LogTemp, Warning, TEXT("%s Died"), *DeadActor->GetName());
}

void ATankGameModeBase::BeginPlay() {

}

void ATankGameModeBase::HandleGameStart() {

}

void ATankGameModeBase::HandleGameOver(bool PlayerWon) {

}
