#include "ShootTraceComponent.h"

#include "DrawDebugHelpers.h"

void UShootTraceComponent::Fire(const FVector& SpawnLocation, const FRotator&, AActor* Owner, USceneComponent*) {
	FHitResult OutHit;
	const auto& Start = SpawnLocation;
	const auto End = ((FVector::ForwardVector * 1000.f) + Start);

	FCollisionQueryParams CollisionParams;
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, true);

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams)) {
		if (OutHit.bBlockingHit && GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("You are hitting: %s"), *OutHit.GetActor()->GetName()));
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Purple, FString::Printf(TEXT("Impact Point: %s"), *OutHit.ImpactPoint.ToString()));
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("Normal Point: %s"), *OutHit.ImpactNormal.ToString()));
		}
	}
}
