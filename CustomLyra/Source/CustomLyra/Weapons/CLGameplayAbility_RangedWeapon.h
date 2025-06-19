#pragma once

#include "CoreMinimal.h"
#include "CustomLyra/Equipment/CLGameplayAbility_FromEquipment.h"
#include "CLGameplayAbility_RangedWeapon.generated.h"

class UCLRangedWeaponInstance;

UENUM(blueprintType)
enum class ECLAbilityTargetingSource : uint8 
{
	CameraTowardsFocus = 0,
};

UCLASS()
class UCLGameplayAbility_RangedWeapon : public UCLGameplayAbility_FromEquipment
{
	GENERATED_BODY()
public:
	struct FRangedWeaponFiringInput
	{
		FVector StartTrace = FVector::ZeroVector;
		FVector EndAim = FVector::ZeroVector;
		FVector AimDir = FVector::ZeroVector;
		UCLRangedWeaponInstance* WeaponData = nullptr;
		bool bCanPlayBulletFX = false;
	};

	UCLGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	void StartRangedWeaponTargeting();

	UFUNCTION(BlueprintImplementableEvent)
	void OnRangeWeaponTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData);

	void PerformLocalTargeting(TArray<FHitResult>& OutHits);
	UCLRangedWeaponInstance* GetWeaponInstance();
	FTransform GetTargetingTransform(APawn* SourcePawn, ECLAbilityTargetingSource Source);
	FVector GetWeaponTargetingSourceLocation() const;

	void TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData, TArray<FHitResult>& OutHits);
	FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHits);
	FHitResult DoSingleBulletTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHits);

	void AddAdditionalTraceIgnoreActors(FCollisionQueryParams& TraceParam) const;
	ECollisionChannel DetermineTraceChannel(FCollisionQueryParams& TraceParam, bool bIsSimulated) const;

	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag);
};