#include "TankBase.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraShake.h"

#include "ToonTanks/Components/PawnMovementComponentBase.h"

ATankBase::ATankBase() {
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ATankBase::BeginPlay() {
	APawnBase::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());

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

void ATankBase::Rotate(float Value) {
	const float RotateAmount = Value * RotateSpeed * GetWorld()->DeltaTimeSeconds;
	const auto Rotation = FRotator(0, RotateAmount, 0);
	AddActorLocalRotation(FQuat(Rotation), true);
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

	if (PlayerController) {
		FHitResult TraceHitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, TraceHitResult);
		FVector HitLocation = TraceHitResult.ImpactPoint;

		RotateTurretToTarget(HitLocation);
	}
}

void ATankBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATankBase::MoveForward);
	PlayerInputComponent->BindAxis("Turn", this, &ATankBase::Rotate);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATankBase::CheckFire);
}
