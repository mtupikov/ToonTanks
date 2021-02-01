#pragma once

#include "GameFramework/DamageType.h"

class AActor;

namespace AmmunitionUtils {

void DamageActorsInSphere(AActor* Causer, float AreaDamage, float AreaDamageDistance, TSubclassOf<UDamageType> DamageType);

} // end namespace AmmunitionUtils
