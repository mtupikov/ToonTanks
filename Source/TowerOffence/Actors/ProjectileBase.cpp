#include "ProjectileBase.h"

#include "Camera/CameraShake.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystemComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "TowerOffence/Pawns/PawnBase.h"

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

	BlowUp();
}

void AProjectileBase::BlowUp() {
	UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, GetActorLocation());

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

	Destroy();
}

float AProjectileBase::GetFireRate() const {
	return FireRate;
}

bool AProjectileBase::IsHoming() const {
	return bIsHoming;
}
