#include "Explosion.h"

#include "PhysicsEngine/RadialForceComponent.h"

AExplosion::AExplosion() {
	PrimaryActorTick.bCanEverTick = false;
	InitialLifeSpan = 1.0f;

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("Radial Force Component"));
	RootComponent = RadialForceComponent;
}

void AExplosion::BeginPlay() {
	Super::BeginPlay();

	RadialForceComponent->FireImpulse();
}
