#include "ProjectileBase.h"

#include "Camera/CameraShake.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AProjectileBase::AProjectileBase() {
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	RootComponent = ProjectileMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = MovementSpeed;
	ProjectileMovement->MaxSpeed = MovementSpeed;

	TrailParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail Particle"));
	TrailParticle->SetupAttachment(RootComponent);
}

void AProjectileBase::BeginPlay() {
	Super::BeginPlay();

	UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	GetWorld()->GetTimerManager().SetTimer(LifeSpanTimerHandle, this, &AProjectileBase::BlowUp, LifeSpanTime, false);
}

void AProjectileBase::SetHomingTarget(USceneComponent* Target) {
	ProjectileMovement->HomingTargetComponent = Target;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->bInitialVelocityInLocalSpace = true;
	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->ProjectileGravityScale = 0.05f;

	LifeSpanTime = 7.0f;
}

void AProjectileBase::OnHit(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse,
	const FHitResult& Hit
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

	BlowUp();
}

void AProjectileBase::BlowUp() {
	UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, GetActorLocation());
	Destroy();
}
