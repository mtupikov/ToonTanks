#include "TurretBase.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "ToonTanks/Pawns/TankBase.h"
#include "ToonTanks/Components/ShootProjectileComponent.h"

ATurretBase::ATurretBase() {}

void ATurretBase::Tick(float DeltaTime) {
	APawnBase::Tick(DeltaTime);

	if (!PlayerPawn || ReturnDistanceToPlayer() > FireRange || !PlayerPawn->IsAlive()) {
		RotateIdle();
		return;
	}

	RotateTurretToTarget(PlayerPawn->GetTargetLocation(), MaximumLeftRelativeRotation, MaximumRightRelativeRotation, true);
}

void ATurretBase::BeginPlay() {
	APawnBase::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &ATurretBase::CheckFireCondition, GetFireRate(), true);
	SelectPlayerPawn();

	if (auto* ProjectileShootComponent = Cast<UShootProjectileComponent>(GetShootComponent())) {
		ProjectileShootComponent->SetProjectileIsHoming(bHasHomingProjectile);
	}
}

void ATurretBase::HandleDestruction() {
	APawnBase::HandleDestruction();

	Destroy();
}

void ATurretBase::CheckFireCondition() {
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
		Fire(PlayerPawn->GetTurretMesh());
	}
}

void ATurretBase::SelectPlayerPawn() {
	auto* PlayerRawPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerRawPawn) {
		return;
	}

	PlayerPawn = Cast<ATankBase>(PlayerRawPawn);
}

void ATurretBase::RotateIdle() {
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

float ATurretBase::ReturnDistanceToPlayer() {
	if (!PlayerPawn) {
		return 0.0f;
	}

	return FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());
}
