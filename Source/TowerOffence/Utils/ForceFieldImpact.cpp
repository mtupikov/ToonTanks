#include "ForceFieldImpact.h"

#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"

#include "TowerOffence/Actors/ForceFieldBase.h"

void UForceFieldImpact::Init(
	AForceFieldBase* InOwner,
	const FVector& ImpactPoint
) {
	Owner = InOwner;
	// setup timeline

	FOnTimelineFloat OnTimelineCallback;
	FOnTimelineEventStatic OnTimelineFinishedCallback;

	const auto TimelineName = FString::Printf(TEXT("Impact Animation Timeline %d"), GetUniqueID());
	ImpactAnimationTimeline = NewObject<UTimelineComponent>(Owner, FName(TimelineName));
	ImpactAnimationTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	ImpactAnimationTimeline->SetNetAddressable();

	ImpactAnimationTimeline->SetLooping(false);
	ImpactAnimationTimeline->SetTimelineLength(Owner->ImpactTime);
	ImpactAnimationTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	ImpactAnimationTimeline->SetPlaybackPosition(0.0f, false);

	OnTimelineCallback.BindUFunction(this, FName(TEXT("TimelineCallback")));
	OnTimelineFinishedCallback.BindUFunction(this, FName(TEXT("TimelineFinishedCallback")));
	ImpactAnimationTimeline->AddInterpFloat(Owner->FloatCurve, OnTimelineCallback);
	ImpactAnimationTimeline->SetTimelineFinishedFunc(OnTimelineFinishedCallback);

	ImpactAnimationTimeline->RegisterComponent();

	// setup new mesh

	const auto SphereName = FString::Printf(TEXT("Impact Sphere %d"), GetUniqueID());
	ImpactSphere = NewObject<UStaticMeshComponent>(Owner->ImpactMesh, *SphereName);
	ImpactSphere->SetStaticMesh(Owner->ImpactMesh);
	ImpactSphere->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	ImpactSphere->SetupAttachment(Owner->RootComponent);
	ImpactSphere->RegisterComponentWithWorld(GetWorld());

	DynamicMaterial = UMaterialInstanceDynamic::Create(Owner->ImpactMesh->GetMaterial(0), Owner);
	DynamicMaterial->SetVectorParameterValue(FName(TEXT("Position")), ImpactPoint);
	ImpactSphere->SetMaterial(0, DynamicMaterial);

	ImpactAnimationTimeline->PlayFromStart();
}

void UForceFieldImpact::TimelineCallback(float Value) {
	DynamicMaterial->SetScalarParameterValue(FName(TEXT("Radius")), Value * Owner->ImpactRadius);
	DynamicMaterial->SetScalarParameterValue(FName(TEXT("Hardness")), Value);
}

void UForceFieldImpact::TimelineFinishedCallback() {
	ImpactSphere->UnregisterComponent();
	ImpactSphere->DetachFromParent();
	Owner->RemoveFinishedImpact(GetUniqueID());
}

void UForceFieldImpact::Tick(float DeltaTime) {
	ImpactAnimationTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
}
