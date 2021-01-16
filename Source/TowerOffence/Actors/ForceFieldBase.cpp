#include "ForceFieldBase.h"

#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "Kismet/KismetMathLibrary.h"

#include "ProjectileBase.h"
#include "TowerOffence/Components/HealthComponent.h"
#include "TowerOffence/Utils/ForceFieldImpact.h"

AForceFieldBase::AForceFieldBase() {
	PrimaryActorTick.bCanEverTick = true;

	ForceFieldCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Force Field Collision"));
	ForceFieldCollision->OnComponentBeginOverlap.AddDynamic(this, &AForceFieldBase::OnBeginOverlap);
	ForceFieldCollision->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	RootComponent = ForceFieldCollision;

	ForceFieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Force Field Mesh"));
	ForceFieldMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	ForceFieldMesh->SetupAttachment(ForceFieldCollision);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

	OnTakeAnyDamage.AddDynamic(this, &AForceFieldBase::OnTakeDamage);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ImpactMeshAsset(TEXT("StaticMesh'/Game/Assets/Meshes/SM_ForceFieldImpact.SM_ForceFieldImpact'"));
	if (ImpactMeshAsset.Succeeded()) {
		ImpactMesh = ImpactMeshAsset.Object;
	}
}

void AForceFieldBase::OnTakeDamage(
	AActor* DamagedActor,
	float Damage,
	const UDamageType* DamageType,
	AController* InstigatedBy,
	AActor* DamageCauser
) {
	if (Damage == 0
		|| !DamagedActor
		|| !DamageCauser
		|| !DamageType
		|| !InstigatedBy) {
		return;
	}

	const auto DisintegrastionAmount = (Damage / 10) * DisintegrationAmountPerHit;
	CurrentDisintegrationAmount += DisintegrastionAmount;
	DynamicForceFieldMaterial->SetScalarParameterValue(FName(TEXT("Amount")), CurrentDisintegrationAmount);
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

	if (!DynamicForceFieldMaterial) {
		DynamicForceFieldMaterial = UMaterialInstanceDynamic::Create(ForceFieldMaterial, ForceFieldMesh);
		ForceFieldMesh->SetMaterial(0, DynamicForceFieldMaterial);
	}

	NewImpact->Init(
		this,
		SweepResult.ImpactPoint
	);
	ActiveImpacts.Add(NewImpact->GetUniqueID(), NewImpact);

	Projectile->OnHit(OtherComp, this, OverlappedComp, {}, SweepResult);
}

void AForceFieldBase::TimelineCallback(float Value) {
	if (DisintegrationAnimationTimeline->IsReversing()) {
		CurrentDisintegrationAmount = FMath::Clamp(CurrentDisintegrationAmount - Value / 10, 0.0f, 100.0f);
	} else {
		CurrentDisintegrationAmount += Value / 1000;
	}

	DynamicForceFieldMaterial->SetScalarParameterValue(FName(TEXT("Amount")), CurrentDisintegrationAmount);
}

void AForceFieldBase::TimelineFinishedCallback() {
	if (UKismetMathLibrary::NearlyEqual_FloatFloat(CurrentDisintegrationAmount, 0.000f)
		|| UKismetMathLibrary::Greater_FloatFloat(HealthComponent->GetHealth(), 0.0f)) {
		SetActorHiddenInGame(false);
	} else {
		SetActorHiddenInGame(true);
	}
}

void AForceFieldBase::Activate() {
	HealthComponent->ResetHealth();
	DisintegrationAnimationTimeline->Reverse();
}

void AForceFieldBase::Deactivate() {
	DisintegrationAnimationTimeline->PlayFromStart();
}

bool AForceFieldBase::IsActive() const {
	return !IsHidden() && UKismetMathLibrary::Greater_FloatFloat(HealthComponent->GetHealth(), 0.0f);
}

void AForceFieldBase::BeginPlay() {
	Super::BeginPlay();

	ForceFieldMaterial = ForceFieldMesh->GetMaterial(0);

	FOnTimelineFloat OnTimelineCallback;
	FOnTimelineEventStatic OnTimelineFinishedCallback;

	DisintegrationAnimationTimeline = NewObject<UTimelineComponent>(this, FName("Disintegration Animation Timeline"));
	DisintegrationAnimationTimeline->CreationMethod = EComponentCreationMethod::Native;
	DisintegrationAnimationTimeline->SetNetAddressable();

	DisintegrationAnimationTimeline->SetLooping(false);
	DisintegrationAnimationTimeline->SetTimelineLength(DisintegrationTime);
	DisintegrationAnimationTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	DisintegrationAnimationTimeline->SetPlaybackPosition(0.0f, false);

	OnTimelineCallback.BindUFunction(this, FName(TEXT("TimelineCallback")));
	OnTimelineFinishedCallback.BindUFunction(this, FName(TEXT("TimelineFinishedCallback")));
	DisintegrationAnimationTimeline->AddInterpFloat(DisintegrationFloatCurve, OnTimelineCallback);
	DisintegrationAnimationTimeline->SetTimelineFinishedFunc(OnTimelineFinishedCallback);
	DisintegrationAnimationTimeline->RegisterComponent();
}

void AForceFieldBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	DisintegrationAnimationTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
}

void AForceFieldBase::RemoveFinishedImpact(uint32 Key) {
	ActiveImpacts.Remove(Key);
}
