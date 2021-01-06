#include "PawnWithProjectileBase.h"

#include "ToonTanks/Actors/ProjectileBase.h"

void APawnWithProjectileBase::Fire() {
	if (!ProjectileClass || !GetTurretMesh()) {
		return;
	}

	const auto SpawnLocation = GetFireSpawnPoint()->GetComponentLocation();
	const auto SpawnRotation = GetFireSpawnPoint()->GetComponentRotation();
	AProjectileBase* TempProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation);
	TempProjectile->SetOwner(this);
}
