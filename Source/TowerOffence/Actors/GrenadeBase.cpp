#include "GrenadeBase.h"

#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Explosion.h"
#include "TowerOffence/Utils/AmmunitionUtils.h"

AGrenadeBase::AGrenadeBase() {
	GrenadeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	GrenadeMesh->OnComponentHit.AddDynamic(this, &AGrenadeBase::OnHit);
	GrenadeMesh->SetSimulatePhysics(true);

	RootComponent = GrenadeMesh;
}

void AGrenadeBase::BeginPlay() {
	AAmmunitionBase::BeginPlay();

	if (LaunchSound) {
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	}

	GetWorld()->GetTimerManager().SetTimer(LifeSpanTimerHandle, this, &AGrenadeBase::Detonate, LifeSpanTime, false);
}

void AGrenadeBase::SetThrowPower(float Power) {
	const auto Speed = Power * MaxGrenadeSpeed;
	const auto ShootDirection = GetActorRotation().Vector();
	const auto Impulse = ShootDirection * Speed;
	GrenadeMesh->SetPhysicsLinearVelocity(Impulse);
}

void AGrenadeBase::Detonate() {
	if (HitSound) {
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}

	if (HitParticle) {
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, GetActorLocation());
	}

	AmmunitionUtils::DamageActorsInSphere(this, AreaDamage, AreaDamageDistance, DamageType);

	if (Explosion) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		GetWorld()->SpawnActor<AExplosion>(Explosion, GetActorLocation(), GetActorRotation(), SpawnParams);
	}

	Destroy();
}

void AGrenadeBase::OnHit(
	UPrimitiveComponent*,
	AActor* OtherActor,
	UPrimitiveComponent*,
	FVector,
	const FHitResult&
) {
	const auto* MyOwner = GetOwner();
	if (!MyOwner) {
		return;
	}

	if (!OtherActor || OtherActor == this || OtherActor == MyOwner) {
		return;
	}

	if (auto* OtherAmmunition = Cast<AAmmunitionBase>(OtherActor)) {
		GetWorld()->GetTimerManager().ClearTimer(LifeSpanTimerHandle);
		OtherAmmunition->Detonate();
		Detonate();
	}	
}
