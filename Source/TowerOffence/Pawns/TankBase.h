#pragma once

#include "CoreMinimal.h"
#include "PawnBase.h"
#include "TankBase.generated.h"

class AHUDBase;
class UMatineeCameraShake;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class TOWEROFFENCE_API ATankBase : public APawnBase {
	GENERATED_BODY()

public:
	ATankBase();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void HandleDestruction() override;

	UFUNCTION(BlueprintCallable)
	float GetForceFieldCurrentLifetime() const;

	UFUNCTION(BlueprintCallable)
	float GetForceFieldCurrentTimeout() const;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnForceFieldDestroyed();

	void RotateBase(float Value);
	void RotateTurret(float Value);
	void MoveForward(float Value);
	void BeginFire();
	void EndFire();
	void RealeseFire();
	void RequestForceFieldActivation();
	void RequestForceFieldDeactivation();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<UMatineeCameraShake> CameraDeathShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float BaseRotationSpeed = 200.0f;

	FTimerHandle FireRateTimerHandle;
	FTimerHandle SingleFireRateTimerHandle;
	FTimerHandle ForceFieldLifetimeTimerHandle;
	FTimerHandle ForceFieldTimeoutTimerHandle;
	APlayerController* PlayerController = nullptr;
	AHUDBase* TankHUD = nullptr;
};
