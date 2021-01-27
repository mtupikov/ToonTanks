#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class ATankGameModeBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TOWEROFFENCE_API UHealthComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UHealthComponent();

	float GetDefaultHealth() const;
	float GetHealth() const;

	void ResetHealth();

	DECLARE_EVENT_OneParam(UHealthComponent, FHealthChangedEvent, float)
	FHealthChangedEvent& OnHealthChanged() {
		return HealthChangedEvent;
	}

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void TakeDamage(
		AActor* DamagedActor,
		float Damage,
		const UDamageType* DamageType,
		AController* InstigatedBy,
		AActor* DamageCauser
	);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float DefaultHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float Health = 0.0f;

	ATankGameModeBase* GameMode = nullptr;
	FHealthChangedEvent HealthChangedEvent;
};
