#include "HitscanBase.h"

#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "TowerOffence/Pawns/MineBase.h"
#include "TowerOffence/Pawns/PawnBase.h"
#include "TowerOffence/Actors/ForceFieldBase.h"

AHitscanBase::AHitscanBase() {
}

void AHitscanBase::Fire(const FVector& AimDirection, const FVector& StartLocation) {
	if (LaunchSound) {
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	}

	const auto RandomSeed = FMath::Rand();
	FRandomStream WeaponRandomStream(RandomSeed);
	const float SpreadCone = FMath::DegreesToRadians(FireSpread * 0.5);
	const auto ShootDir = WeaponRandomStream.VRandCone(AimDirection, SpreadCone, SpreadCone);
	const auto EndTrace = StartLocation + ShootDir * Range;
	const auto Impacts = WeaponTrace(StartLocation, EndTrace);

	ProcessInstantHit(Impacts, StartLocation, EndTrace, ShootDir, FireSpread);
}

TArray<FHitResult> AHitscanBase::WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const {
	static const auto WeaponFireTag = FName(TEXT("WeaponTrace"));

	FCollisionQueryParams TraceParams(WeaponFireTag, true, GetInstigator());
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(this);

	TArray<FHitResult> Hits;
	GetWorld()->LineTraceMultiByChannel(Hits, TraceFrom, TraceTo, ECC_GameTraceChannel1, TraceParams);

	return Hits;
}

void AHitscanBase::ProcessInstantHit(
	const TArray<FHitResult>& Impacts,
	const FVector& Origin,
	const FVector& End,
	const FVector& ShootDir,
	float Spread
) {
	FVector EndPoint;
	FVector TraceEnd;
	AActor* Actor = nullptr;

	for (const auto& Impact : Impacts) {
		TraceEnd = Impact.TraceEnd;
		EndPoint = Impact.GetActor() ? Impact.ImpactPoint : End;
		Actor = Impact.GetActor();

		if (!Actor) {
			continue;
		}

		auto* ForceField = Cast<AForceFieldBase>(Actor);
		if (ForceField) {
			if (ForceField->IsActive()) {
				ForceField->CreateImpact(EndPoint);
			} else {
				Actor = nullptr;
				continue;
			}
		}

		break;
	}

	if (TrailParticle && TrailParticle->IsValid()) {
		const auto DistanceIsNan = FGenericPlatformMath::IsNaN(FVector::Dist(Origin, End));
		const auto ShootRotation = ShootDir.Rotation();

		FVector BeamEnd;
		if (DistanceIsNan) {
			BeamEnd = Origin + ShootDir * 2000.0f;
		} else {
			BeamEnd = End;
		}

		auto* Muzzle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			MuzzleFlashParticle,
			Origin,
			ShootRotation,
			FVector(1.0f, 1.0f, 1.0f),
			true,
			EPSCPoolMethod::AutoRelease,
			true
		);

		auto* Effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			TrailParticle,
			Origin,
			ShootRotation,
			FVector(1.0f, 1.0f, 1.0f),
			true,
			true,
			ENCPoolMethod::AutoRelease,
			true
		);

		Effect->SetNiagaraVariableVec3(TEXT("BeamEnd"), BeamEnd);
	}

	if (!Actor) {
		return;
	}
	
	//FlushPersistentDebugLines(Actor->GetWorld());
	//DrawDebugLine(Actor->GetWorld(), Origin, TraceEnd, FColor::Black, true, 1.0f, 10.f);

	UGameplayStatics::ApplyDamage(Actor, Damage, GetOwner()->GetInstigatorController(), this, DamageType);

	if (HitSound) {
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, EndPoint);
	}
	if (HitParticle) {
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, EndPoint);
	}
}
