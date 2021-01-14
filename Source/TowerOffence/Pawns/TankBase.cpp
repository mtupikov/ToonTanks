#include "TankBase.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraShake.h"

#include "TowerOffence/Components/PawnMovementComponentBase.h"

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

	if (GetPawnMovementComponent() && (GetPawnMovementComponent()->UpdatedComponent == RootComponent)) {
		GetPawnMovementComponent()->SetMoveSpeed(MoveSpeed);
	}
}

void ATankBase::HandleDestruction() {
	APawnBase::HandleDestruction();
	GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(CameraDeathShake);

	SetIsAlive(false);
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
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

void ATankBase::CheckFire() {
	if (GetWorld()->GetTimerManager().IsTimerActive(FireRateTimerHandle)) {
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, GetFireRate(), false);
	Fire();
}

void ATankBase::Tick(float DeltaTime) {
	APawnBase::Tick(DeltaTime);
}

void ATankBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATankBase::MoveForward);
	PlayerInputComponent->BindAxis("TurnBase", this, &ATankBase::RotateBase);
	PlayerInputComponent->BindAxis("TurnTurret", this, &ATankBase::RotateTurret);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATankBase::CheckFire);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATankBase::CheckFire);
}
