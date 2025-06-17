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
	void PerformLocalTargeting(TArray<FHitResult>& OutHits);
	UCLRangedWeaponInstance* GetWeaponInstance();
	FTransform GetTargetingTransform(APawn* SourcePawn, ECLAbilityTargetingSource Source);
	FVector GetWeaponTargetingSourceLocation() const;
};