#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShootProjectileComponent.generated.h"

class AProjectileBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOWEROFFENCE_API UShootProjectileComponent : public UActorComponent {
	GENERATED_BODY()

public:
	void Fire(const FVector& SpawnLocation, const FRotator& SpawnRotation, AActor* Owner, USceneComponent* Target = nullptr);

	void SetProjectile(TSubclassOf<AProjectileBase> Projectile);

	float GetFireRate() const;

private:
	TSubclassOf<AProjectileBase> ProjectileClass;
};
