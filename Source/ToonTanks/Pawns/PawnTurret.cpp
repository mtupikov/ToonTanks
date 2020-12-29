#include "PawnTurret.h"

#include "Kismet/GameplayStatics.h"

APawnTurret::APawnTurret() {
	
}

void APawnTurret::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void APawnTurret::BeginPlay() {
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &APawnTurret::CheckFireCondition, FireRate, true);
}

void APawnTurret::CheckFireCondition() {

}
