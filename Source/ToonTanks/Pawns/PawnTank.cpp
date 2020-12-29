#include "PawnTank.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

APawnTank::APawnTank() {
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void APawnTank::BeginPlay() {
	Super::BeginPlay();
}

void APawnTank::CalculateMoveInput(float Value) {
	MoveDirection = FVector(Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0, 0);
}

void APawnTank::CalculateRotateInput(float Value) {
	float RotateAmount = Value * RotateSpeed * GetWorld()->DeltaTimeSeconds;
	auto Rotation = FRotator(0, RotateAmount, 0);
	RotationDirection = FQuat(Rotation);
}

void APawnTank::Move() {
	AddActorLocalOffset(MoveDirection, true);
}

void APawnTank::Rotate() {
	AddActorLocalRotation(RotationDirection, true);
}

void APawnTank::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	Rotate();
	Move();
}

void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APawnTank::CalculateMoveInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawnTank::CalculateRotateInput);
}
