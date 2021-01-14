#include "ForceFieldBase.h"

#include "Components/SphereComponent.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "Kismet/KismetMathLibrary.h"

#include "ProjectileBase.h"

AForceFieldBase::AForceFieldBase() {
	PrimaryActorTick.bCanEverTick = true;

	ForceFieldCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Force Field Collision"));
	ForceFieldCollision->OnComponentBeginOverlap.AddDynamic(this, &AForceFieldBase::OnBeginOverlap);
	ForceFieldCollision->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	RootComponent = ForceFieldCollision;

	ForceFieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Force Field Mesh"));
	ForceFieldMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	ForceFieldMesh->SetupAttachment(ForceFieldCollision);
}

void AForceFieldBase::BeginPlay() {
	Super::BeginPlay();

	FOnTimelineFloat OnTimelineCallback;
	
	if (FloatCurve) {
		ImpactAnimationTimeline = NewObject<UTimelineComponent>(this, FName("Impact Animation Timeline"));
		ImpactAnimationTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		this->BlueprintCreatedComponents.Add(ImpactAnimationTimeline);
		ImpactAnimationTimeline->SetNetAddressable();

		ImpactAnimationTimeline->SetLooping(false);
		ImpactAnimationTimeline->SetTimelineLength(0.5f);
		ImpactAnimationTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
		ImpactAnimationTimeline->SetPlaybackPosition(0.0f, false);

		OnTimelineCallback.BindUFunction(this, FName(TEXT("TimelineCallback")));
		ImpactAnimationTimeline->AddInterpFloat(FloatCurve, OnTimelineCallback);

		ImpactAnimationTimeline->RegisterComponent();
	}

	ForceFieldMaterial = ForceFieldMesh->GetMaterial(0);
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

	auto* Projectile = Cast<AProjectileBase>(OtherActor);
	if (!Projectile) {
		return;
	}

	if (ImpactAnimationTimeline) {
		DynamicMaterial = UMaterialInstanceDynamic::Create(ForceFieldMaterial, this);
		DynamicMaterial->SetVectorParameterValue(FName(TEXT("Position")), SweepResult.ImpactPoint);
		ForceFieldMesh->SetMaterial(0, DynamicMaterial);

		ImpactAnimationTimeline->PlayFromStart();
	}

	FVector ImpactPoint;
	const auto Distance = ForceFieldCollision->GetDistanceToCollision(Projectile->GetActorLocation(), ImpactPoint);
	const auto IsNearZero = UKismetMathLibrary::NearlyEqual_FloatFloat(Distance, 0.000f);
	
	if (!IsNearZero) {
		Projectile->BlowUp();
	}
}

void AForceFieldBase::TimelineCallback(float Value) {
	DynamicMaterial->SetScalarParameterValue(FName(TEXT("Radius")), Value * 100.0f);
	DynamicMaterial->SetScalarParameterValue(FName(TEXT("Hardness")), Value);
}

void AForceFieldBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (ImpactAnimationTimeline) {
		ImpactAnimationTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
	}
}