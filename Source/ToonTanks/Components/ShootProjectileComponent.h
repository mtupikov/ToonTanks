#pragma once

#include "CoreMinimal.h"
#include "ShootComponent.h"
#include "ShootProjectileComponent.generated.h"

class AProjectileBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOONTANKS_API UShootProjectileComponent : public UShootComponent {
	GENERATED_BODY()

public:
	virtual void Fire(const FVector& SpawnLocation, const FRotator& SpawnRotation, AActor* Owner, USceneComponent* Target = nullptr) override;

	void SetProjectile(TSubclassOf<AProjectileBase> Projectile);
	void SetProjectileIsHoming(bool Value);

private:
	TSubclassOf<AProjectileBase> ProjectileClass;
	bool bIsHomingProjectile = false;
};
