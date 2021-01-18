#include "MissleProjectile.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "TowerOffence/Pawns/PawnBase.h"

void AMissleProjectile::DestroyProjectile() {
	TArray<AActor*> OutActors;
	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic),
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody)
	};
	const TArray<AActor*> ActorsToIgnore{ this };
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		GetActorLocation(),
		AreaDamageDistance,
		ObjectTypes,
		nullptr,
		ActorsToIgnore,
		OutActors
	);

	for (auto* Actor : OutActors) {
		if (GetOwner()) {
			auto* Pawn = Cast<APawnBase>(Actor);
			if (Pawn && Pawn->ForceFieldIsActive()) {
				continue;
			}

			UGameplayStatics::ApplyDamage(Actor, AreaDamage, GetOwner()->GetInstigatorController(), this, DamageType);
		}
	}

	AProjectileBase::DestroyProjectile();
}
