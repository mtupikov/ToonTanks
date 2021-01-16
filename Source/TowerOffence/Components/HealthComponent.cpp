#include "HealthComponent.h"

#include "TowerOffence/GameModes/TankGameModeBase.h"

#include "Kismet/GameplayStatics.h"

UHealthComponent::UHealthComponent() {
	PrimaryComponentTick.bCanEverTick = false;
}

float UHealthComponent::GetDefaultHealth() const {
	return DefaultHealth;
}

float UHealthComponent::GetHealth() const {
	return Health;
}

void UHealthComponent::ResetHealth() {
	Health = DefaultHealth;
}

void UHealthComponent::BeginPlay() {
	Super::BeginPlay();

	Health = DefaultHealth;
	GameMode = Cast<ATankGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
}

void UHealthComponent::TakeDamage(
	AActor* DamagedActor,
	float Damage,
	const UDamageType* DamageType,
	AController* InstigatedBy,
	AActor* DamageCauser
) {
	if (Health == 0
		|| Damage == 0
		|| !DamagedActor
		|| !DamageCauser
		|| !DamageType
		|| !InstigatedBy) {
		return;
	}

	if (!GameMode) {
		UE_LOG(LogTemp, Error, TEXT("Health Component does not have valid GameMode pointer"));
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	if (Health == 0) {
		GameMode->ActorDied(GetOwner());
	}
}
