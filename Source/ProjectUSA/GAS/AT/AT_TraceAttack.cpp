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
#include "Interface/USADamageableInterface.h"

#include "Component/USACharacterAttackComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"

UAT_TraceAttack::UAT_TraceAttack()
{

}


UAT_TraceAttack* UAT_TraceAttack::GetNewAbilityTask_TraceAttack
(UGameplayAbility* OwningAbility, const FAttackTraceInfos& InAttackTraceData, const FVector& InTargetVector)
{
	UAT_TraceAttack* MyObj = NewAbilityTask<UAT_TraceAttack>(OwningAbility);

	MyObj->AttackTraceData = &InAttackTraceData;
	MyObj->TargetVector = InTargetVector;

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

	if (IsValid(MyCharacter) == false)
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

	IUSAAttackableInterface* AttackableInterface = nullptr;

	if (Ability != nullptr)
	{
		AttackableInterface = Cast<IUSAAttackableInterface>(Ability->GetAvatarActorFromActorInfo());
	}

	while (CurrentAttackTraceIndex < AttackTraceData->AttackTraceInfos.Num()
		&& PrevAttackTraceTime + SMALL_NUMBER >= NextSpawnTime)
	{
		// 공격 방향 설정
		FVector FinalAttackDirection = MyCharacter->GetActorForwardVector();
		FVector FinalAttackDirectionRight = MyCharacter->GetActorRightVector();
		FVector FinalAttackDirectionUp = FVector::UpVector;

		if (AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].bIsDirectToTarget == true
			&& TargetVector.Length() > SMALL_NUMBER)
		{
			FinalAttackDirection = TargetVector;
			FinalAttackDirectionRight = FVector::CrossProduct(FVector::UpVector, FinalAttackDirection);
			FinalAttackDirectionUp = FVector::CrossProduct(FinalAttackDirection, FinalAttackDirectionRight);
		}

		// 인스턴트 / 지속형 판단
		if (AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].AttackDuration > SMALL_NUMBER)
		{
			if (AttackableInterface != nullptr
				&& IsValid (AttackableInterface->GetUSACharacterAttackComponent()) == true)
			{
				AttackableInterface->GetUSACharacterAttackComponent()
					->RegisterUSAAttackInfo(AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex],
						MyCharacter->GetActorLocation(),
						FinalAttackDirection, FinalAttackDirectionRight, FinalAttackDirectionUp);
			}

			CurrentAttackTraceIndex += 1;

			if (CurrentAttackTraceIndex < AttackTraceData->AttackTraceInfos.Num())
			{
				NextSpawnTime = AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].AttackTime;
			}

			continue;
		}


		FVector AttackStartTraceLocation = MyCharacter->GetActorLocation();
		AttackStartTraceLocation += MyCharacter->GetActorForwardVector() * AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].OffsetTraceLocation.X;
		AttackStartTraceLocation += MyCharacter->GetActorRightVector() * AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].OffsetTraceLocation.Y;
		AttackStartTraceLocation += MyCharacter->GetActorUpVector() * AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].OffsetTraceLocation.Z;

		FVector AttackEndTraceLocation = AttackStartTraceLocation;
		AttackEndTraceLocation += FinalAttackDirection * AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].OffsetTraceEndLocation.X;
		AttackEndTraceLocation += FinalAttackDirectionRight * AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].OffsetTraceEndLocation.Y;
		AttackEndTraceLocation += FinalAttackDirectionUp * AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].OffsetTraceEndLocation.Z;

		float AttackDamage = AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].AttackDamage;
		float AttackTraceRadius = AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].AttackTraceRadius;

		// 싱글 트래이스
		if (AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].bIsUsingSigleTrace == true)
		{
			FHitResult HitResult;
			UKismetSystemLibrary::SphereTraceSingle
			(GetWorld(),
				AttackStartTraceLocation,
				AttackEndTraceLocation,
				AttackTraceRadius,
				UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Pawn),
				false,
				IgnoreActors,
				EDrawDebugTrace::None,
				HitResult,
				true,
				FLinearColor::Red,
				FLinearColor::Green,
				0.5f);

			if (HitResult.bBlockingHit == true)
			{
				//ACharacter* OutCharacter = Cast <ACharacter>(HitResult.GetActor());

				//if (OutCharacter == nullptr)
				//{
				//	continue;
				//}

				IUSADamageableInterface* USADamageableInterface = Cast<IUSADamageableInterface>(HitResult.GetActor());

				if (USADamageableInterface == nullptr)
				{
					return;
				}

				TSubclassOf<UDamageType> AttackDamageType = AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].AttackDamageType;
				FPointDamageEvent AttackDamageEvent = FPointDamageEvent(AttackDamage, HitResult, AttackDirection, AttackDamageType);

				USADamageableInterface->ApplyDamageHitNiagaraEffect(MyCharacter->GetController(), MyCharacter,
					AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].AttackHitNiagaraSystemObject,
					AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].AttackHitNiagaraSystemObjectRandomRatioX,
					AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].AttackHitNiagaraSystemObjectRandomRatioY,
					AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].AttackHitNiagaraSystemObjectRandomRatioZ);

				USADamageableInterface->TakeDamage(AttackDamage, AttackDamageEvent, MyCharacter->GetController(), MyCharacter);
			}
		}
		// 멀티 트래이스
		else
		{
			TArray<FHitResult> HitResults;
			UKismetSystemLibrary::SphereTraceMulti
			(GetWorld(),
				AttackStartTraceLocation,
				AttackEndTraceLocation,
				AttackTraceRadius,
				UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Pawn),
				false,
				IgnoreActors,
				EDrawDebugTrace::None,
				HitResults,
				true,
				FLinearColor::Red,
				FLinearColor::Green,
				0.5f);

			for (FHitResult HitResultOne : HitResults)
			{
				//ACharacter* OutCharacter = Cast <ACharacter>(HitResultOne.GetActor());

				//if (OutCharacter == nullptr)
				//{
				//	continue;
				//}

				IUSADamageableInterface* USADamageableInterface = Cast<IUSADamageableInterface>(HitResultOne.GetActor());

				if (USADamageableInterface == nullptr)
				{
					continue;
				}

				TSubclassOf<UDamageType> AttackDamageType = AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].AttackDamageType;
				FPointDamageEvent AttackDamageEvent = FPointDamageEvent(AttackDamage, HitResultOne, AttackDirection, AttackDamageType);

				USADamageableInterface->ApplyDamageHitNiagaraEffect(MyCharacter->GetController(), MyCharacter,
					AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].AttackHitNiagaraSystemObject,
					AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].AttackHitNiagaraSystemObjectRandomRatioX,
					AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].AttackHitNiagaraSystemObjectRandomRatioY,
					AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].AttackHitNiagaraSystemObjectRandomRatioZ);

				USADamageableInterface->TakeDamage(AttackDamage, AttackDamageEvent, MyCharacter->GetController(), MyCharacter);
			}
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

