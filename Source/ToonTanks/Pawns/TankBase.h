#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "TankBase.generated.h"

class UMatineeCameraShake;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class TOONTANKS_API ATankBase : public APawnBase {
	GENERATED_BODY()

public:
	ATankBase();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void HandleDestruction() override;

protected:
	virtual void BeginPlay() override;

private:
	void Rotate(float Value);
	void MoveForward(float Value);
	void CheckFire();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<UMatineeCameraShake> CameraDeathShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float RotateSpeed = 200.0f;

	FTimerHandle FireRateTimerHandle;
	APlayerController* PlayerController = nullptr;
};
