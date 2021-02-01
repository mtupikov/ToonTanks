#include "AmmunitionBase.h"

#include "GameFramework/DamageType.h"

AAmmunitionBase::AAmmunitionBase() {
	PrimaryActorTick.bCanEverTick = false;
}

float AAmmunitionBase::GetFireRate() const {
	return FireRate;
}

const FVector& AAmmunitionBase::GetSpawnPoint() const {
	return SpawnPoint;
}

void AAmmunitionBase::Detonate() {}

void AAmmunitionBase::BeginPlay() {
	Super::BeginPlay();

	SpawnPoint = GetActorLocation();
}
