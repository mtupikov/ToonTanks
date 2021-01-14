#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnBase.generated.h"

class AProjectileBase;
class UPawnMovementComponentBase;
class USceneComponent;
class UStaticMeshComponent;
class UHealthComponent;
class UShootComponent;

UENUM()
enum class EShootType : uint8 {
	None                   UMETA(DisplayName = "None"),
	TraceBullet            UMETA(DisplayName = "Trace Bullet"),
	ProjectileRocket       UMETA(DisplayName = "Projectile Rocket"),
	ProjectileHomingRocket UMETA(DisplayName = "Projectile Homing Rocket")
};

UCLASS()
class TOWEROFFENCE_API APawnBase : public APawn {
	GENERATED_BODY()

public:
	APawnBase();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void HandleDestruction();

	static float MaximumRotationAngle();

	float GetFireRate() const;
	float GetTurretRotationSpeed() const;

	bool IsAlive() const;
	void SetIsAlive(bool Value);

	UStaticMeshComponent* GetTurretMesh() const;
	USceneComponent* GetFireSpawnPoint() const;
	UPawnMovementComponentBase* GetPawnMovementComponent() const;
	UShootComponent* GetShootComponent() const;

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
	UShootComponent* ShootComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* DeathSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* DeathParticle = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	EShootType ShootType = EShootType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float TurretRotationSpeed = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float FireRate = 2.0f;

	FRotator InitialRotator;
	bool bIsPawnAlive = true;
};
