#include "CLGameplayAbility_RangedWeapon.h"
#include "CLRangedWeaponInstance.h"
#include "CustomLyra/Physics/CLCollisionChannels.h"
#include "CustomLyra/AbilitySystem/CLGameplayAbilityTargetData_SingleTargetHit.h"
#include "AbilitySystemComponent.h"

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
			newTargetData->CartridgeId = cartridgeID;
			targetData.Add(newTargetData);
		}
	}

	OnTargetDataReadyCallback(targetData, FGameplayTag());
}

void UCLGameplayAbility_RangedWeapon::PerformLocalTargeting(TArray<FHitResult>& OutHits)
{
	APawn* avatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());

	UCLRangedWeaponInstance* weaponData = GetWeaponInstance();
	if (IsValid(avatarPawn) == true && avatarPawn->IsLocallyControlled() == true && IsValid(weaponData) == true)
	{
		FRangedWeaponFiringInput inputData;
		inputData.WeaponData = weaponData;
		inputData.bCanPlayBulletFX = true;

		const FTransform targetTransform = GetTargetingTransform(avatarPawn, ECLAbilityTargetingSource::CameraTowardsFocus);

		inputData.AimDir = targetTransform.GetUnitAxis(EAxis::X);
		inputData.StartTrace = targetTransform.GetTranslation();
		inputData.EndAim = inputData.StartTrace + inputData.AimDir * weaponData->MaxDamageRange;

		static float debugThickness = 2.0f;
		DrawDebugLine(GetWorld(), inputData.StartTrace, inputData.StartTrace + (inputData.AimDir * 100.0f), FColor::Yellow, false, 10.0f, 0, debugThickness);

		TraceBulletsInCartridge(inputData, OutHits);
	}
}

UCLRangedWeaponInstance* UCLGameplayAbility_RangedWeapon::GetWeaponInstance()
{
	return Cast<UCLRangedWeaponInstance>(GetAssociatedEquipment());
}

FTransform UCLGameplayAbility_RangedWeapon::GetTargetingTransform(APawn* SourcePawn, ECLAbilityTargetingSource Source)
{
	AController* controller = SourcePawn->GetController();
	if (IsValid(controller) == false)
	{
		return FTransform{};
	}

	// camera의 near plane 보정을 위한 focal
	double focalDistance = 1024.0f;
	FVector focalLoc;
	FVector camLoc;
	FRotator camRot;

	APlayerController* playerController = Cast<APlayerController>(controller);
	playerController->GetPlayerViewPoint(camLoc, camRot);

	FVector aimDir = camRot.Vector().GetSafeNormal();
	focalLoc = camLoc + aimDir * focalDistance;

	FVector weaponLoc = GetWeaponTargetingSourceLocation();
	// ((weaponLoc - focalLoc) | aimDir ) * aimDir 를 하면 사영벡터가 나온다. 
	// 사영벡터 공식은 (V . U) / (U * U) * U
	// 이는 aimDir가 단위벡터 이므로 (V . U / U * U) * U 에서 
	// 분모 U * U가 사실상 1이고 그렇다면
	// V . U * U 하면 사영벡터가 나옴 
	// 우리는 focalLoc 에서 weaponLoc 까지 빠지는 vector를 구해야 한다
	// 그래야 실제 weaponLoc에서 ray를 쏠 수 있다.
	// U가 단위벡터이므로 V의 크기만큼 빠지는 벡터로 만들어질 것이다. V * 1 == 크기는 V만큼 일것이므로
	FVector finalCamLoc = focalLoc + ((weaponLoc - focalLoc) | aimDir) * aimDir;

	return FTransform(camRot, finalCamLoc);
}

FVector UCLGameplayAbility_RangedWeapon::GetWeaponTargetingSourceLocation() const
{
	APawn* avatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());
	return avatarPawn->GetActorLocation();
}

void UCLGameplayAbility_RangedWeapon::TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData, TArray<FHitResult>& OutHits)
{
	UCLRangedWeaponInstance* weaponData = InputData.WeaponData;

	const FVector bulletDir = InputData.AimDir;
	const FVector endTrace = InputData.StartTrace + (bulletDir * weaponData->MaxDamageRange);

	FVector hitLocation = endTrace;

	TArray<FHitResult> allImpacts;
	FHitResult impact = DoSingleBulletTrace(InputData.StartTrace, endTrace, weaponData->BulletTraceWeaponRadius, false, allImpacts);

	const AActor* hitActor = impact.GetActor();
	if (IsValid(hitActor) == true)
	{
		if (allImpacts.Num() > 0)
		{
			OutHits.Append(allImpacts);
		}
	}
}



FHitResult UCLGameplayAbility_RangedWeapon::WeaponTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHits)
{
	TArray<FHitResult> hitResults;

	FCollisionQueryParams traceParam(SCENE_QUERY_STAT(WeaponTrace), true, GetAvatarActorFromActorInfo());
	traceParam.bReturnPhysicalMaterial = true;
	traceParam.bIgnoreBlocks = false;

	AddAdditionalTraceIgnoreActors(traceParam);

	const ECollisionChannel traceChannel = DetermineTraceChannel(traceParam, bIsSimulated);
	if (SweepRadius > 0.0f)
	{
		GetWorld()->SweepMultiByChannel(hitResults, StartTrace, EndTrace, FQuat::Identity, traceChannel, FCollisionShape::MakeSphere(SweepRadius), traceParam);
	}
	else
	{
		GetWorld()->LineTraceMultiByChannel(hitResults, StartTrace, EndTrace, traceChannel, traceParam);
	}

	FHitResult hit{ ForceInit };
	if (hitResults.Num() > 0)
	{
		for (FHitResult& curHitResult : hitResults)
		{
			auto pred = [&curHitResult](const FHitResult& InOther)
				{
					return InOther.HitObjectHandle == curHitResult.HitObjectHandle;
				};

			if (OutHits.ContainsByPredicate(pred) == false)
			{
				OutHits.Add(curHitResult);
			}
		}

		hit = OutHits.Last();
	}
	else
	{
		hit.TraceStart = StartTrace;
		hit.TraceEnd = EndTrace;
	}

	return hit;
}

int32 FindFirstPawnHitResult(const TArray<FHitResult>& HitResults)
{
	for (int32 index = 0; index < HitResults.Num(); ++index)
	{
		const FHitResult& curHitResult = HitResults[index];
		if (curHitResult.HitObjectHandle.DoesRepresentClass(APawn::StaticClass()) == true)
		{
			return index;
		}
		else
		{
			AActor* hitActor = curHitResult.HitObjectHandle.FetchActor();

			if (IsValid(hitActor) == true && hitActor->GetAttachParentActor() != nullptr && Cast<APawn>(hitActor->GetAttachParentActor()) != nullptr)
			{
				return index;
			}
		}
	}

	return INDEX_NONE;
}

FHitResult UCLGameplayAbility_RangedWeapon::DoSingleBulletTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHits)
{
	FHitResult impact;

	if (FindFirstPawnHitResult(OutHits) == INDEX_NONE)
	{
		impact = WeaponTrace(StartTrace, EndTrace, 0.0f, bIsSimulated, OutHits);
	}

	if (FindFirstPawnHitResult(OutHits) == INDEX_NONE)
	{
		if (SweepRadius > 0.0f)
		{
			TArray<FHitResult> sweepHits;
			impact = WeaponTrace(StartTrace, EndTrace, SweepRadius, bIsSimulated, sweepHits);

			int32 firstPawnIndex = FindFirstPawnHitResult(sweepHits);
		}
	}

	return FHitResult();
}

void UCLGameplayAbility_RangedWeapon::AddAdditionalTraceIgnoreActors(FCollisionQueryParams& TraceParam) const
{
	if (AActor* avatar = GetAvatarActorFromActorInfo())
	{
		TArray<AActor*> attachedActors;

		avatar->GetAttachedActors(attachedActors);
		TraceParam.AddIgnoredActors(attachedActors);
	}
}

ECollisionChannel UCLGameplayAbility_RangedWeapon::DetermineTraceChannel(FCollisionQueryParams& TraceParam, bool bIsSimulated) const
{
	return CL_TraceChannel_Weapon;
}

void UCLGameplayAbility_RangedWeapon::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag)
{
	UAbilitySystemComponent* abilitySystemComponent = CurrentActorInfo->AbilitySystemComponent.Get();

	if (const FGameplayAbilitySpec* spec = abilitySystemComponent->FindAbilitySpecFromHandle(CurrentSpecHandle))
	{
		FGameplayAbilityTargetDataHandle localTargetDataHandle = MoveTemp(const_cast<FGameplayAbilityTargetDataHandle&>(InData));

		if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo) == true)
		{
			OnRangeWeaponTargetDataReady(localTargetDataHandle);
		}
		else
		{
			K2_EndAbility();
		}
	}
}