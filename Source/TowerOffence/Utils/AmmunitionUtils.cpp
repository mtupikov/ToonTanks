#include "AmmunitionUtils.h"

#include "Containers/Array.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "TowerOffence/Pawns/PawnBase.h"

namespace AmmunitionUtils {

void DamageActorsInSphere(AActor* Causer, float AreaDamage, float AreaDamageDistance, TSubclassOf<UDamageType> DamageType) {
	TArray<AActor*> OutActors;
	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic),
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody)
	};
	const TArray<AActor*> ActorsToIgnore{ Causer };
	UKismetSystemLibrary::SphereOverlapActors(
		Causer->GetWorld(),
		Causer->GetActorLocation(),
		AreaDamageDistance,
		ObjectTypes,
		nullptr,
		ActorsToIgnore,
		OutActors
	);

	for (auto* Actor : OutActors) {
		if (Causer->GetOwner()) {
			auto* Pawn = Cast<APawnBase>(Actor);
			if (Pawn && Pawn->ForceFieldIsActive()) {
				continue;
			}

			UGameplayStatics::ApplyDamage(Actor, AreaDamage, Causer->GetOwner()->GetInstigatorController(), Causer, DamageType);
		}
	}
}

} // end namespace AmmunitionUtils
