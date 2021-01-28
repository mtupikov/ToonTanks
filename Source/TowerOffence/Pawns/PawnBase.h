#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnBase.generated.h"

class AAmmunitionBase;
class AForceFieldBase;
class UPawnMovementComponentBase;
class USceneComponent;
class UStaticMeshComponent;
class UHealthComponent;
class UShootAmmunitionComponent;

UCLASS()
class TOWEROFFENCE_API APawnBase : public APawn {
	GENERATED_BODY()

public:
	APawnBase();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void HandleDestruction();

	static float MaximumRotationAngle();

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	float GetFireRate() const;
	float GetTurretRotationSpeed() const;

	float GetForceFieldLifetime() const;
	float GetForceFieldTimeout() const;

	bool IsAlive() const;
	void SetIsAlive(bool Value);

	void ActivateForceField();
	void DeactivateForceField();
	bool ForceFieldIsActive() const;

	UStaticMeshComponent* GetTurretMesh() const;
	USceneComponent* GetFireSpawnPoint() const;
	UPawnMovementComponentBase* GetPawnMovementComponent() const;
	UShootAmmunitionComponent* GetShootComponent() const;

	FRotator GetTurretRotation() const;
	FRotator GetTurretInitialRotation() const;
	FVector GetTurretLocation() const;

protected:
	struct ResultRotators {
		FRotator ResultRotator;
		FRotator LeftMaxRotator;
		FRotator RightMaxRotator;
	};

	virtual void BeginPlay() override;
	void RotateTurretToTarget(
		const FVector& TargetLocation,
		float LeftMaxAngle = APawnBase::MaximumRotationAngle(),
		float RightMaxAngle = APawnBase::MaximumRotationAngle(),
		bool IgnoreIfOutOfRange = false
	);
	void RotateTurret(const FRotator& Rotation);

	ResultRotators RotatorsToLocation(
		const FRotator& ActorRotator,
		const FVector& TargetLocation,
		float LeftMaxAngle,
		float RightMaxAngle
	);

	virtual void Fire(USceneComponent* Target = nullptr);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* FireSpawnPoint = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UPawnMovementComponentBase* MovementComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UShootAmmunitionComponent* ShootComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AForceFieldBase> ForceFieldBP = nullptr;

	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* DeathSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* DeathParticle = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"), meta = (MustImplement = AProjectileBase))
	TSubclassOf<AAmmunitionBase> AmmunitionClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float TurretRotationSpeed = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float ForceFieldLifetime = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float ForceFieldTimeout = 5.0f;

	AForceFieldBase* ForceField = nullptr;
	FRotator InitialRotator;
	bool bIsPawnAlive = true;
};
