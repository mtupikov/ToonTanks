#include "ProjectileBase.h"

#include "Camera/CameraShake.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "Kismet/GameplayStatics.h"

AProjectileBase::AProjectileBase() {
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	RootComponent = ProjectileMesh;

	TrailParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail Particle"));
	TrailParticle->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
}

void AProjectileBase::BeginPlay() {
	AAmmunitionBase::BeginPlay();

	if (LaunchSound) {
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	}

	SpawnPoint = GetActorLocation();
	GetWorld()->GetTimerManager().SetTimer(LifeSpanTimerHandle, this, &AProjectileBase::DestroyProjectile, LifeSpanTime, false);
}

void AProjectileBase::OnHit(
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

	GetWorld()->GetTimerManager().ClearTimer(LifeSpanTimerHandle);

	UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwner->GetInstigatorController(), this, DamageType);
	GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(CameraHitShake);

	DestroyProjectile();
}

FVector AProjectileBase::GetSpawnPoint() const {
	return FVector();
}

void AProjectileBase::DestroyProjectile() {
	if (HitSound) {
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}
	if (HitParticle) {
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, GetActorLocation());
	}

	Destroy();
}
