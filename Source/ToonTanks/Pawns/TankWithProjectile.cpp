#include "TankWithProjectile.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraShake.h"

#include "ToonTanks/Components/PawnMovementComponentBase.h"

ATankWithProjectile::ATankWithProjectile() {
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ATankWithProjectile::BeginPlay() {
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());

	if (GetPawnMovementComponent() && (GetPawnMovementComponent()->UpdatedComponent == RootComponent)) {
		GetPawnMovementComponent()->SetMoveSpeed(MoveSpeed);
	}
}

void ATankWithProjectile::HandleDestruction() {
	Super::HandleDestruction();
	GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(CameraDeathShake);

	SetIsAlive(false);
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

void ATankWithProjectile::Rotate(float Value) {
	const float RotateAmount = Value * RotateSpeed * GetWorld()->DeltaTimeSeconds;
	const auto Rotation = FRotator(0, RotateAmount, 0);
	AddActorLocalRotation(FQuat(Rotation), true);
}

void ATankWithProjectile::MoveForward(float Value) {
	if (GetPawnMovementComponent() && (GetPawnMovementComponent()->UpdatedComponent == RootComponent)) {
		GetPawnMovementComponent()->AddInputVector(GetActorForwardVector() * Value);
	}
}

void ATankWithProjectile::CheckFire() {
	if (GetWorld()->GetTimerManager().IsTimerActive(FireRateTimerHandle)) {
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, GetFireRate(), false);
	Fire();
}

void ATankWithProjectile::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (PlayerController) {
		FHitResult TraceHitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, TraceHitResult);
		FVector HitLocation = TraceHitResult.ImpactPoint;

		RotateTurretToTarget(HitLocation);
	}
}

void ATankWithProjectile::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATankWithProjectile::MoveForward);
	PlayerInputComponent->BindAxis("Turn", this, &ATankWithProjectile::Rotate);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATankWithProjectile::CheckFire);
}
