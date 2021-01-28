#include "TankBase.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraShake.h"

#include "TowerOffence/Actors/HitscanBase.h"
#include "TowerOffence/Actors/MissleProjectile.h"
#include "TowerOffence/Actors/HomingMissleProjectile.h"
#include "TowerOffence/Actors/ForceFieldBase.h"
#include "TowerOffence/Components/PawnMovementComponentBase.h"
#include "TowerOffence/Components/ShootAmmunitionComponent.h"
#include "TowerOffence/HUD/HUDBase.h"
#include "TowerOffence/Utils/CrosshairType.h"
#include "TowerOffence/Utils/CrosshairManager.h"

ATankBase::ATankBase() {
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(GetTurretMesh());
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->bEnableCameraRotationLag = true;
	SpringArmComponent->CameraLagSpeed = 7.0f;
	SpringArmComponent->CameraRotationLagSpeed = 3.0f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ATankBase::BeginPlay() {
	APawnBase::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());
	PlayerController->bShowMouseCursor = false;
	
	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(false);
	PlayerController->SetInputMode(InputMode);

	auto* BaseHUD = PlayerController->GetHUD();
	TankHUD = Cast<AHUDBase>(BaseHUD);
	ensure(TankHUD);

	if (GetPawnMovementComponent() && (GetPawnMovementComponent()->UpdatedComponent == RootComponent)) {
		GetPawnMovementComponent()->SetMoveSpeed(MoveSpeed);
	}

	const auto Ammunition = GetShootComponent()->GetAmmunition();
	if (auto* Ptr = Ammunition.GetDefaultObject()) {
		CrosshairType Type = CrosshairType::None;
		if (Cast<AHitscanBase>(Ptr)) {
			Type = CrosshairType::Bullet;
		} else if (Cast<AHomingMissleProjectile>(Ptr) || Cast<AMissleProjectile>(Ptr)) {
			Type = CrosshairType::Rocket;
		}

		if (auto* CrosshairManager = TankHUD->GetCrosshairManager()) {
			CrosshairManager->SetCrosshairType(Type);
		} else {
			UE_LOG(LogTemp, Warning, TEXT("Cannot get crosshair manager"));
		}
	}

	auto* FField = GetForceField();
	FField->OnDestroyed().AddUFunction(this, FName("OnForceFieldDestroyed"));
}

void ATankBase::HandleDestruction() {
	APawnBase::HandleDestruction();

	auto& TimerManager = GetWorld()->GetTimerManager();
	GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(CameraDeathShake);
	TimerManager.ClearTimer(FireRateTimerHandle);
	TimerManager.ClearTimer(SingleFireRateTimerHandle);
	TimerManager.ClearTimer(ForceFieldLifetimeTimerHandle);
	TimerManager.ClearTimer(ForceFieldTimeoutTimerHandle);

	SetIsAlive(false);
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

float ATankBase::GetForceFieldCurrentLifetime() const {
	auto& TimerManager = GetWorld()->GetTimerManager();
	if (!TimerManager.IsTimerActive(ForceFieldLifetimeTimerHandle)) {
		return 0.0f;
	}

	const auto Elapsed = TimerManager.GetTimerElapsed(ForceFieldLifetimeTimerHandle);
	return FMath::GetMappedRangeValueClamped({ 0.0f, GetForceFieldLifetime() }, { 0.0f, 1.0f }, Elapsed);
}

float ATankBase::GetForceFieldCurrentTimeout() const {
	auto& TimerManager = GetWorld()->GetTimerManager();
	if (!TimerManager.IsTimerActive(ForceFieldTimeoutTimerHandle)) {
		return 0.0f;
	}

	const auto Elapsed = TimerManager.GetTimerElapsed(ForceFieldTimeoutTimerHandle);
	return FMath::GetMappedRangeValueClamped({ 0.0f, GetForceFieldTimeout() }, { 0.0f, 1.0f }, Elapsed);
}

void ATankBase::RotateBase(float Value) {
	const float RotateAmount = Value * BaseRotationSpeed * GetWorld()->DeltaTimeSeconds;
	const auto Rotation = FRotator(0, RotateAmount, 0);
	AddActorLocalRotation(FQuat(Rotation), true);
}

void ATankBase::RotateTurret(float Value) {
	const auto CurrentRotation = GetTurretRotation();
	const auto NewYaw = CurrentRotation.Yaw + Value * GetTurretRotationSpeed() * GetWorld()->DeltaTimeSeconds;
	const auto RotateTo = FRotator(CurrentRotation.Pitch, NewYaw, CurrentRotation.Roll);
	APawnBase::RotateTurret(RotateTo);
}

void ATankBase::MoveForward(float Value) {
	if (GetPawnMovementComponent() && (GetPawnMovementComponent()->UpdatedComponent == RootComponent)) {
		GetPawnMovementComponent()->AddInputVector(GetActorForwardVector() * Value);
	}
}

void ATankBase::BeginFire() {
	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &ATankBase::RealeseFire, GetFireRate(), true);
}

void ATankBase::EndFire() {
	GetWorld()->GetTimerManager().ClearTimer(FireRateTimerHandle);

	if (!GetWorld()->GetTimerManager().IsTimerActive(SingleFireRateTimerHandle)) {
		RealeseFire();
	}
}

void ATankBase::RealeseFire() {
	GetWorld()->GetTimerManager().SetTimer(SingleFireRateTimerHandle, GetFireRate(), false);

	Fire();
}

void ATankBase::RequestForceFieldActivation() {
	auto& TimerManager = GetWorld()->GetTimerManager();

	if (TimerManager.IsTimerActive(ForceFieldTimeoutTimerHandle) || TimerManager.IsTimerActive(ForceFieldLifetimeTimerHandle)) {
		return;
	}

	ActivateForceField();
	TimerManager.ClearTimer(ForceFieldTimeoutTimerHandle);
	TimerManager.SetTimer(ForceFieldLifetimeTimerHandle, this, &ATankBase::RequestForceFieldDeactivation, GetForceFieldLifetime(), false);
}

void ATankBase::RequestForceFieldDeactivation() {
	auto& TimerManager = GetWorld()->GetTimerManager();

	DeactivateForceField();
	TimerManager.ClearTimer(ForceFieldLifetimeTimerHandle);
	TimerManager.SetTimer(ForceFieldTimeoutTimerHandle, GetForceFieldTimeout(), false);
}

void ATankBase::OnForceFieldDestroyed() {
	auto& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.ClearTimer(ForceFieldLifetimeTimerHandle);
	TimerManager.SetTimer(ForceFieldTimeoutTimerHandle, GetForceFieldTimeout(), false);
}

void ATankBase::Tick(float DeltaTime) {
	APawnBase::Tick(DeltaTime);
}

void ATankBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATankBase::MoveForward);
	PlayerInputComponent->BindAxis("TurnBase", this, &ATankBase::RotateBase);
	PlayerInputComponent->BindAxis("TurnTurret", this, &ATankBase::RotateTurret);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATankBase::BeginFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ATankBase::EndFire);
	PlayerInputComponent->BindAction("ActivateForceField", IE_Pressed, this, &ATankBase::RequestForceFieldActivation);
}
