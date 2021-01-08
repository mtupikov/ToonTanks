#include "MineBase.h"

#include "Kismet/GameplayStatics.h"

#include "ToonTanks/Pawns/TankBase.h"
#include "ToonTanks/Components/HealthComponent.h"

#include "DrawDebugHelpers.h"

AMineBase::AMineBase() {
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->OnComponentHit.AddDynamic(this, &AMineBase::OnHit);
	RootComponent = BaseMesh;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

void AMineBase::BeginPlay() {
	Super::BeginPlay();

	SelectPlayerPawn();
}

void AMineBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!PlayerPawn || DistanceToPlayer() > DetectDistance || !PlayerPawn->IsAlive()) {
		GetWorld()->GetTimerManager().ClearTimer(DetonationTimerHandle);
		bPlayerDetected = false;
		return;
	}

	if (bPlayerDetected) {
		return;
	}

	if (DetectSound) {
		UGameplayStatics::PlaySoundAtLocation(this, DetectSound, GetActorLocation());
	}

	bPlayerDetected = true;
	GetWorld()->GetTimerManager().SetTimer(DetonationTimerHandle, this, &AMineBase::BlowUp, DetonationTime, false);
}

void AMineBase::BlowUp() {
	if (DetonationSound && DetonationParticle) {
		UGameplayStatics::PlaySoundAtLocation(this, DetonationSound, GetActorLocation());
		UGameplayStatics::SpawnEmitterAtLocation(this, DetonationParticle, GetActorLocation());
	}

	TArray<AActor*> OutActors;
	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes{
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic),
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody)
	};
	const TArray<AActor*> ActorsToIgnore{ this };
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		GetActorLocation(),
		DetectDistance,
		ObjectTypes,
		nullptr,
		ActorsToIgnore,
		OutActors
	);

	for (auto* Actor : OutActors) {
		if (!GetOwner()) {
			continue;
		}

		UGameplayStatics::ApplyDamage(Actor, AreaDamage, GetOwner()->GetInstigatorController(), this, DamageType);
	}

	Destroy();
}

void AMineBase::SelectPlayerPawn() {
	auto* PlayerRawPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerRawPawn) {
		return;
	}

	PlayerPawn = Cast<ATankBase>(PlayerRawPawn);
}

float AMineBase::DistanceToPlayer() {
	if (!PlayerPawn) {
		return 0.0f;
	}

	return FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());
}

void AMineBase::OnHit(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse,
	const FHitResult& Hit
) {
	UE_LOG(LogTemp, Warning, TEXT("OnHit"));

	const auto* MyOwner = GetOwner();
	if (!MyOwner) {
		return;
	}

	if (!OtherActor || OtherActor == this || OtherActor == MyOwner) {
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("OnHit good"));

	GetWorld()->GetTimerManager().ClearTimer(DetonationTimerHandle);
	UGameplayStatics::ApplyDamage(OtherActor, OnHitDamage, MyOwner->GetInstigatorController(), this, DamageType);
	
	BlowUp();
}
