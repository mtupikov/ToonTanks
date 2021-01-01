#include "TankGameModeBase.h"

#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"

#include "Kismet/GameplayStatics.h"

ATankGameModeBase::ATankGameModeBase() {}

void ATankGameModeBase::ActorDied(AActor* DeadActor) {
	if (DeadActor == PlayerTank) {
		PlayerTank->HandleDestruction();
		HandleGameOver(false);
	} else if (APawnTurret* DestroyedTurret = Cast<APawnTurret>(DeadActor)) {
		DestroyedTurret->HandleDestruction();
		TargetTurretsCount -= 1;

		if (TargetTurretsCount == 0) {
			HandleGameOver(true);
		}
	}
}

void ATankGameModeBase::BeginPlay() {
	Super::BeginPlay();
	HandleGameStart();
}

void ATankGameModeBase::HandleGameStart() {
	TArray<AActor*> TurretActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnTurret::StaticClass(), TurretActors);
	TargetTurretsCount = TurretActors.Num();
	PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));

	GameStart();
}

void ATankGameModeBase::HandleGameOver(bool PlayerWon) {
	GameOver(PlayerWon);
}
