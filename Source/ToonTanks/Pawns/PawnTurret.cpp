#include "PawnTurret.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "PawnTank.h"

APawnTurret::APawnTurret() {
	
}

void APawnTurret::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!PlayerPawn || ReturnDistanceToPlayer() > FireRange || !PlayerPawn->IsAlive()) {
		return;
	}

	RotateTurretToTarget(PlayerPawn->GetTargetLocation(), MaximumLeftRelativeRotation, MaximumRightRelativeRotation, true);
}

void APawnTurret::BeginPlay() {
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &APawnTurret::CheckFireCondition, FireRate, true);
	SelectPlayerPawn();
}

void APawnTurret::HandleDestruction() {
	Super::HandleDestruction();

	Destroy();
}

void APawnTurret::CheckFireCondition() {
	if (!PlayerPawn) {
		return;
	}

	const auto Rotators = RotatorsToLocation(
		GetTurretInitialRotation(),
		PlayerPawn->GetTargetLocation(),
		MaximumLeftRelativeRotation,
		MaximumRightRelativeRotation
	);
	if (!UKismetMathLibrary::InRange_FloatFloat(Rotators.ResultRotator.Yaw, Rotators.LeftMaxRotator.Yaw, Rotators.RightMaxRotator.Yaw, false, false)) {
		return;
	}

	if (ReturnDistanceToPlayer() <= FireRange && PlayerPawn->IsAlive()) {
		Fire();
	}
}

void APawnTurret::SelectPlayerPawn() {
	auto* PlayerRawPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerRawPawn) {
		return;
	}

	PlayerPawn = Cast<APawnTank>(PlayerRawPawn);
}

float APawnTurret::ReturnDistanceToPlayer() {
	if (!PlayerPawn) {
		return 0.0f;
	}

	return FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());
}
