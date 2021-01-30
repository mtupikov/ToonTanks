#include "HomingMissleProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"

AHomingMissleProjectile::AHomingMissleProjectile() : AMissleProjectile() {
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->bInitialVelocityInLocalSpace = true;
	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->ProjectileGravityScale = 0.02f;
	ProjectileMovement->HomingAccelerationMagnitude = 7000.0f;
}

void AHomingMissleProjectile::SetHomingTarget(USceneComponent* Target) {
	if (!Target) {
		UE_LOG(LogTemp, Warning, TEXT("Passing null target to %s"), *GetName());
		return;
	}

	ProjectileMovement->HomingTargetComponent = Target;
}
