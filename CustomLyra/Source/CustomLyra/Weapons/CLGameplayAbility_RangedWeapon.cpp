#include "CLGameplayAbility_RangedWeapon.h"

UCLGameplayAbility_RangedWeapon::UCLGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer) : Super{ObjectInitializer}
{
}

void UCLGameplayAbility_RangedWeapon::StartRangedWeaponTargeting()
{
	AActor* avatarActor = CurrentActorInfo->AvatarActor.Get();

	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();

	TArray<FHitResult> foundHits;
	PerformLocalTargeting(foundHits);

	FGameplayAbilityTargetDataHandle targetData;
	targetData.UniqueId = 0;

	if (foundHits.Num() > 0)
	{
		// 샷건같은경우 총알이 여러개 들어있으니, 그 탄약을 카트리지라 생각
		const int32 cartridgeID = FMath::Rand();
		for (const FHitResult& result : foundHits)
		{
			FCLGameplayAbilityTargetData_SingleTargetHit* newTargetData = new FCLGameplayAbilityTargetData_SingleTargetHit();
			newTargetData->HitResult = result;
			newTargetData->CartridgeID = cartridgeID;
			targetData.Add(newTargetData);
		}
	}
}
