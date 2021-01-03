#include "PawnTurret.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "PawnTank.h"

APawnTurret::APawnTurret() {
	
}

void APawnTurret::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!PlayerPawn || ReturnDistanceToPlayer() > FireRange || !PlayerPawn->IsAlive()) {
		RotateIdle();
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

void APawnTurret::RotateIdle() {
	const auto LeftMax = GetTurretInitialRotation().Add(0, -MaximumLeftRelativeRotation, 0).GetNormalized();
	const auto RightMax = GetTurretInitialRotation().Add(0, MaximumRightRelativeRotation, 0).GetNormalized();
	const auto CurrentYaw = GetTurretRotation().Yaw;

	const auto RotateToIfNotNear = [this, &CurrentYaw](const FRotator& Max, bool IsLeft) {
		if (UKismetMathLibrary::NearlyEqual_FloatFloat(CurrentYaw, Max.Yaw, 0.01f)) {
			bIsIdleRotatingLeft = !IsLeft;
		} else {
			const auto NewRotator = FMath::RInterpConstantTo(GetTurretRotation(), Max, GetWorld()->DeltaTimeSeconds, IdleRotationSpeed);
			RotateTurret(NewRotator);
		}
	};

	if (bIsIdleRotatingLeft) {
		RotateToIfNotNear(LeftMax, true);
	} else {
		RotateToIfNotNear(RightMax, false);
	}
}

float APawnTurret::ReturnDistanceToPlayer() {
	if (!PlayerPawn) {
		return 0.0f;
	}

	return FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());
}
