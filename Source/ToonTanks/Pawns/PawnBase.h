#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnBase.generated.h"

class AProjectileBase;
class UMatineeCameraShake;
class UCapsuleComponent;
class UPawnMovementComponentBase;
class USceneComponent;
class UStaticMeshComponent;
class UHealthComponent;

UCLASS()
class TOONTANKS_API APawnBase : public APawn {
	GENERATED_BODY()

public:
	APawnBase();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void HandleDestruction();

protected:
	struct ResultRotators {
		FRotator ResultRotator;
		FRotator LeftMaxRotator;
		FRotator RightMaxRotator;
	};

	static float MaximumRotationAngle();
	
	virtual void BeginPlay() override;
	void RotateTurretToTarget(
		FVector TargetLocation,
		float LeftMaxAngle = 180.0f,
		float RightMaxAngle = 180.0f,
		bool IgnoreIfOutOfRange = false
	);
	void Fire();

	ResultRotators RotatorsToLocation(
		FRotator ActorRotator,
		FVector TargetLocation,
		float LeftMaxAngle,
		float RightMaxAngle
	);
	FRotator GetTurretRotation() const;
	FRotator GetTurretInitialRotation() const;
	FVector GetTurretLocation() const;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<UMatineeCameraShake> CameraDeathShake;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UPawnMovementComponentBase* MovementComponent = nullptr;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Type", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* DeathSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* DeathParticle = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float TurretRotationSpeed = 60.0f;

	FRotator InitialRotator;
};
