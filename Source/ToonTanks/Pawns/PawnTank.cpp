#include "PawnTank.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraShake.h"

#include "ToonTanks/Components/PawnMovementComponentBase.h"

APawnTank::APawnTank() {
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void APawnTank::BeginPlay() {
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());

	if (GetPawnMovementComponent() && (GetPawnMovementComponent()->UpdatedComponent == RootComponent)) {
		GetPawnMovementComponent()->SetMoveSpeed(MoveSpeed);
	}
}

void APawnTank::HandleDestruction() {
	Super::HandleDestruction();
	GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(CameraDeathShake);

	bIsPlayerAlive = false;

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

bool APawnTank::IsAlive() const {
	return bIsPlayerAlive;
}

void APawnTank::Rotate(float Value) {
	const float RotateAmount = Value * RotateSpeed * GetWorld()->DeltaTimeSeconds;
	const auto Rotation = FRotator(0, RotateAmount, 0);
	AddActorLocalRotation(FQuat(Rotation), true);
}

void APawnTank::MoveForward(float Value) {
	if (GetPawnMovementComponent() && (GetPawnMovementComponent()->UpdatedComponent == RootComponent)) {
		GetPawnMovementComponent()->AddInputVector(GetActorForwardVector() * Value);
	}
}

void APawnTank::CheckFire() {
	if (GetWorld()->GetTimerManager().IsTimerActive(FireRateTimerHandle)) {
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, GetFireRate(), false);
	Fire();
}

void APawnTank::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (PlayerController) {
		FHitResult TraceHitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, TraceHitResult);
		FVector HitLocation = TraceHitResult.ImpactPoint;

		RotateTurretToTarget(HitLocation);
	}
}

void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APawnTank::MoveForward);
	PlayerInputComponent->BindAxis("Turn", this, &APawnTank::Rotate);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APawnTank::CheckFire);
}
