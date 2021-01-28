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
	HealthComponent->OnHealthChanged().AddUFunction(this, FName("OnHealthChanged"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ImpactMeshAsset(TEXT("StaticMesh'/Game/Assets/Meshes/SM_ForceFieldImpact.SM_ForceFieldImpact'"));
	if (ImpactMeshAsset.Succeeded()) {
		ImpactMesh = ImpactMeshAsset.Object;
	}
}

void AForceFieldBase::OnHealthChanged(float Health) {
	if (FMath::IsNearlyZero(Health)) {
		ForceFieldDestroyedEvent.Broadcast();
	}

	CurrentDisintegrationAmount = FMath::GetMappedRangeValueClamped(
		{ 0.0f, HealthComponent->GetDefaultHealth() },
		{ 0.0f, RelativeDisintegrationAmountMax },
		HealthComponent->GetDefaultHealth() - Health
	);

	if (!DynamicForceFieldMaterial) {
		CreateDynamicForceFieldMaterial();
	}
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
	
	if (IsInside(Projectile->GetActorLocation()) && IsFarEnough(Projectile->GetSpawnPoint(), 20.0f)) {
		return;
	}

	CreateImpact(SweepResult.ImpactPoint);
	Projectile->OnHit(OtherComp, this, OverlappedComp, {}, SweepResult);
}

void AForceFieldBase::ActivationTimelineCallback(float Value) {
	CurrentDisintegrationAmount = FMath::GetMappedRangeValueClamped({ 0.0f, ActivationTime }, { 0.0f, RealDisintegrationAmountMax }, ActivationTime - Value);
	DynamicForceFieldMaterial->SetScalarParameterValue(FName(TEXT("Amount")), CurrentDisintegrationAmount);
}

void AForceFieldBase::DisintegrationTimelineCallback(float Value) {
	CurrentDisintegrationAmount = FMath::GetMappedRangeValueClamped({ 0.0f, DisintegrationTime }, { 0.0f, RealDisintegrationAmountMax }, Value);
	DynamicForceFieldMaterial->SetScalarParameterValue(FName(TEXT("Amount")), CurrentDisintegrationAmount);
}

void AForceFieldBase::DisintegrationTimelineFinishedCallback() {
	SetActorHiddenInGame(true);
}

void AForceFieldBase::Init(bool ActiveOnSpawn) {
	SetActorHiddenInGame(!ActiveOnSpawn);
}

void AForceFieldBase::Activate() {
	SetActorHiddenInGame(false);
	HealthComponent->ResetHealth();
	ActivationAnimationTimeline->PlayFromStart();
}

void AForceFieldBase::Deactivate() {
	DisintegrationAnimationTimeline->SetPlaybackPosition(CurrentDisintegrationAmount, false);
	DisintegrationAnimationTimeline->Play();
}

void AForceFieldBase::CreateImpact(const FVector& ImpactPoint) {
	const auto ImpactName = FString::Printf(TEXT("Impact %d"), ActiveImpacts.Num());
	auto* NewImpact = NewObject<UForceFieldImpact>(this, UForceFieldImpact::StaticClass(), FName(ImpactName));
	if (!NewImpact) {
		return;
	}

	if (!DynamicForceFieldMaterial) {
		CreateDynamicForceFieldMaterial();
	}

	NewImpact->Init(
		this,
		ImpactPoint
	);
	ActiveImpacts.Add(NewImpact->GetUniqueID(), NewImpact);
}

bool AForceFieldBase::IsInside(const FVector& Point) const {
	FVector ImpactPoint;
	const auto Distance = ForceFieldCollision->GetDistanceToCollision(Point, ImpactPoint);
	return UKismetMathLibrary::NearlyEqual_FloatFloat(Distance, 0.000f);
}

bool AForceFieldBase::IsActive() const {
	return !IsHidden() && UKismetMathLibrary::Greater_FloatFloat(HealthComponent->GetHealth(), 0.0f);
}

void AForceFieldBase::BeginPlay() {
	Super::BeginPlay();

	ForceFieldMaterial = ForceFieldMesh->GetMaterial(0);
	CreateDynamicForceFieldMaterial();

	InitActivationTimeline();
	InitDisintegrationTimeline();
}

void AForceFieldBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	ActivationAnimationTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
	DisintegrationAnimationTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
}

bool AForceFieldBase::IsFarEnough(const FVector& Point, float Distance) const {
	FVector ImpactPoint;
	const auto DistanceToCollision = ForceFieldCollision->GetDistanceToCollision(Point, ImpactPoint);
	return UKismetMathLibrary::GreaterEqual_FloatFloat(DistanceToCollision, Distance);
}

void AForceFieldBase::RemoveFinishedImpact(uint32 Key) {
	ActiveImpacts.Remove(Key);
}

void AForceFieldBase::CreateDynamicForceFieldMaterial() {
	DynamicForceFieldMaterial = UMaterialInstanceDynamic::Create(ForceFieldMaterial, ForceFieldMesh);
	ForceFieldMesh->SetMaterial(0, DynamicForceFieldMaterial);
}

void AForceFieldBase::InitActivationTimeline() {
	FOnTimelineFloat OnTimelineCallback;

	ActivationAnimationTimeline = NewObject<UTimelineComponent>(this, FName("Activation Animation Timeline"));
	ActivationAnimationTimeline->CreationMethod = EComponentCreationMethod::Native;
	ActivationAnimationTimeline->SetNetAddressable();

	ActivationAnimationTimeline->SetLooping(false);
	ActivationAnimationTimeline->SetTimelineLength(ActivationTime);
	ActivationAnimationTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	ActivationAnimationTimeline->SetPlaybackPosition(0.0f, false);

	OnTimelineCallback.BindUFunction(this, FName(TEXT("ActivationTimelineCallback")));
	ActivationAnimationTimeline->AddInterpFloat(ActivationFloatCurve, OnTimelineCallback);
	ActivationAnimationTimeline->RegisterComponent();
}

void AForceFieldBase::InitDisintegrationTimeline() {
	FOnTimelineFloat OnTimelineCallback;
	FOnTimelineEventStatic OnTimelineFinishedCallback;

	DisintegrationAnimationTimeline = NewObject<UTimelineComponent>(this, FName("Disintegration Animation Timeline"));
	DisintegrationAnimationTimeline->CreationMethod = EComponentCreationMethod::Native;
	DisintegrationAnimationTimeline->SetNetAddressable();

	DisintegrationAnimationTimeline->SetLooping(false);
	DisintegrationAnimationTimeline->SetTimelineLength(DisintegrationTime);
	DisintegrationAnimationTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	DisintegrationAnimationTimeline->SetPlaybackPosition(0.0f, false);

	OnTimelineCallback.BindUFunction(this, FName(TEXT("DisintegrationTimelineCallback")));
	OnTimelineFinishedCallback.BindUFunction(this, FName(TEXT("DisintegrationTimelineFinishedCallback")));
	DisintegrationAnimationTimeline->AddInterpFloat(DisintegrationFloatCurve, OnTimelineCallback);
	DisintegrationAnimationTimeline->SetTimelineFinishedFunc(OnTimelineFinishedCallback);
	DisintegrationAnimationTimeline->RegisterComponent();
}
