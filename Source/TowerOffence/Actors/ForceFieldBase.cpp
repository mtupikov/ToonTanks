#include "ForceFieldBase.h"

#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "Kismet/KismetMathLibrary.h"

#include "ProjectileBase.h"
#include "TowerOffence/Components/HealthComponent.h"
#include "TowerOffence/Utils/ForceFieldImpact.h"

AForceFieldBase::AForceFieldBase() {
	PrimaryActorTick.bCanEverTick = false;

	ForceFieldCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Force Field Collision"));
	ForceFieldCollision->OnComponentBeginOverlap.AddDynamic(this, &AForceFieldBase::OnBeginOverlap);
	ForceFieldCollision->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	RootComponent = ForceFieldCollision;

	ForceFieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Force Field Mesh"));
	ForceFieldMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	ForceFieldMesh->SetupAttachment(ForceFieldCollision);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ImpactMeshAsset(TEXT("StaticMesh'/Game/Assets/Meshes/SM_ForceFieldImpact.SM_ForceFieldImpact'"));
	if (ImpactMeshAsset.Succeeded()) {
		ImpactMesh = ImpactMeshAsset.Object;
	}
}

void AForceFieldBase::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
) {
	if (!OtherActor || OtherActor == this) {
		return;
	}

	if (IsHidden()) {
		return;
	}

	auto* Projectile = Cast<AProjectileBase>(OtherActor);
	if (!Projectile) {
		return;
	}
	
	FVector ImpactPoint;
	const auto Distance = ForceFieldCollision->GetDistanceToCollision(Projectile->GetActorLocation(), ImpactPoint);
	const auto IsNearZero = UKismetMathLibrary::NearlyEqual_FloatFloat(Distance, 0.000f);

	if (IsNearZero) {
		return;
	}

	const auto ImpactName = FString::Printf(TEXT("Impact %d"), ActiveImpacts.Num());
	auto* NewImpact = NewObject<UForceFieldImpact>(this, UForceFieldImpact::StaticClass(), FName(ImpactName));
	if (!NewImpact) {
		return;
	}

	NewImpact->Init(
		this,
		SweepResult.ImpactPoint
	);
	ActiveImpacts.Add(NewImpact->GetUniqueID(), NewImpact);

	Projectile->OnHit(OtherComp, this, OverlappedComp, {}, SweepResult);
}

void AForceFieldBase::Activate() {
	SetActorHiddenInGame(false);
}

void AForceFieldBase::Deactivate() {
	SetActorHiddenInGame(true);
}

void AForceFieldBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	for (auto& Pair : ActiveImpacts) {
		if (Pair.Value) {
			Pair.Value->Tick(DeltaTime);
		}
	}
}

void AForceFieldBase::RemoveFinishedImpact(uint32 Key) {
	ActiveImpacts.Remove(Key);
}
