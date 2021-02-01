#include "MissleProjectile.h"

#include "TowerOffence/Utils/AmmunitionUtils.h"

void AMissleProjectile::Detonate() {
	AmmunitionUtils::DamageActorsInSphere(this, AreaDamage, AreaDamageDistance, DamageType);

	AProjectileBase::Detonate();
}
