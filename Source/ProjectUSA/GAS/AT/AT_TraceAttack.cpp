// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AT/AT_TraceAttack.h"

#include "GameFramework/Character.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Debug/DebugDrawComponent.h"

#include "Engine/DamageEvents.h"
#include "GameFramework/DamageType.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

#include "Interface/USAAttackableInterface.h"



UAT_TraceAttack::UAT_TraceAttack()
{

}


UAT_TraceAttack* UAT_TraceAttack::GetNewAbilityTask_TraceAttack(UGameplayAbility* OwningAbility, const FAttackTraceData& InAttackTraceData)
{
	UAT_TraceAttack* MyObj = NewAbilityTask<UAT_TraceAttack>(OwningAbility);

	MyObj->AttackTraceData = &InAttackTraceData;

	MyObj->bIsCancelable = true;

	return MyObj;
}

void UAT_TraceAttack::Activate()
{
	Super::Activate();

	AttackTraceAndSetNextTimer();
	
	SetWaitingOnAvatar();
}

void UAT_TraceAttack::AttackTraceAndSetNextTimer()
{
	ACharacter* MyCharacter = Cast <ACharacter>(GetAvatarActor());

	if (MyCharacter == nullptr)
	{
		return;
	}

	if (AttackTraceData == nullptr)
	{
		return;
	}

	if (CurrentAttackTraceIndex >= AttackTraceData->AttackTraceInfos.Num())
	{
		return;
	}

	float NextSpawnTime = AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].AttackTime;

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTyps;
	TraceObjectTyps.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Init(GetAvatarActor(), 1);

	FVector AttackDirection = MyCharacter->GetActorForwardVector();

	
	while (CurrentAttackTraceIndex < AttackTraceData->AttackTraceInfos.Num()
		&& PrevAttackTraceTime + SMALL_NUMBER >= NextSpawnTime)
	{
		FVector FinalAttackDirection = MyCharacter->GetActorForwardVector();
		FVector FinalAttackDirectionRight = MyCharacter->GetActorRightVector();
		FVector FinalAttackDirectionUp = FVector::UpVector;

		IUSAAttackableInterface* AttackableInterface = nullptr;
		
		if (Ability != nullptr)
		{
			AttackableInterface = Cast<IUSAAttackableInterface>(Ability->GetAvatarActorFromActorInfo());
		}

		if (AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].bIsDirectToTarget == true
			&& AttackableInterface != nullptr)
		{
			FinalAttackDirection = AttackableInterface->GetTargetingDirection();
			FinalAttackDirectionRight = FVector::CrossProduct(FVector::UpVector, FinalAttackDirection);
			FinalAttackDirectionUp = FVector::CrossProduct(FinalAttackDirection, FinalAttackDirectionRight);
		}


		FVector AttackStartTraceLocation = GetAvatarActor()->GetActorLocation();
		AttackStartTraceLocation += GetAvatarActor()->GetActorForwardVector() * AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].OffsetTraceLocation.X;
		AttackStartTraceLocation += GetAvatarActor()->GetActorRightVector() * AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].OffsetTraceLocation.Y;
		AttackStartTraceLocation += GetAvatarActor()->GetActorUpVector() * AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].OffsetTraceLocation.Z;

		FVector AttackEndTraceLocation = AttackStartTraceLocation;
		AttackEndTraceLocation += FinalAttackDirection * AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].OffsetTraceEndLocation.X;
		AttackEndTraceLocation += FinalAttackDirectionRight * AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].OffsetTraceEndLocation.Y;
		AttackEndTraceLocation += FinalAttackDirectionUp * AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].OffsetTraceEndLocation.Z;

		float AttackDamage = AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].AttackDamage;
		float AttackTraceRadius = AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].AttackTraceRadius;

		TArray<FHitResult> HitResults;
		UKismetSystemLibrary::SphereTraceMulti(GetWorld(),
			AttackStartTraceLocation,
			AttackEndTraceLocation,
			AttackTraceRadius,
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Pawn),
			false,
			IgnoreActors,
			EDrawDebugTrace::ForDuration,
			HitResults,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			0.1f);

		for (FHitResult HitResult : HitResults)
		{
			ACharacter* OutCharacter = Cast <ACharacter>(HitResult.GetActor());

			if (OutCharacter == nullptr)
			{
				continue;
			}

			TSubclassOf<UDamageType> AttackDamageType = AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].AttackDamageType;
			FPointDamageEvent AttackDamageEvent = FPointDamageEvent(AttackDamage, HitResult, AttackDirection, AttackDamageType);

			OutCharacter->TakeDamage(AttackDamage, AttackDamageEvent, MyCharacter->GetController(), MyCharacter);
		}

		PrevAttackTraceTime = FMath::Max(PrevAttackTraceTime, NextSpawnTime);

		CurrentAttackTraceIndex += 1;

		if (CurrentAttackTraceIndex < AttackTraceData->AttackTraceInfos.Num())
		{
			NextSpawnTime = AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].AttackTime;
		}
	}

	if (CurrentAttackTraceIndex < AttackTraceData->AttackTraceInfos.Num())
	{
		float TimeDelay = NextSpawnTime - PrevAttackTraceTime;
		PrevAttackTraceTime = NextSpawnTime;

		GetWorld()->GetTimerManager().SetTimer(SpawnActorTimerHandle, this, &UAT_TraceAttack::AttackTraceAndSetNextTimer, TimeDelay, false);
	}
}

