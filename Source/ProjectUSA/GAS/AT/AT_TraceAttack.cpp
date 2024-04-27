// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AT/AT_TraceAttack.h"

#include "GameFramework/Character.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Debug/DebugDrawComponent.h"

#include "Engine/DamageEvents.h"
#include "GameFramework/DamageType.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"


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
		FVector AttackStartTraceLocation = GetAvatarActor()->GetActorLocation();
		AttackStartTraceLocation += GetAvatarActor()->GetActorForwardVector() * AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].OffsetTraceLocation.X;
		AttackStartTraceLocation += GetAvatarActor()->GetActorRightVector() * AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].OffsetTraceLocation.Y;
		AttackStartTraceLocation += GetAvatarActor()->GetActorUpVector() * AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].OffsetTraceLocation.Z;

		FVector AttackEndTraceLocation = AttackStartTraceLocation;
		AttackEndTraceLocation += AttackDirection * 1.0f;

		float AttackTraceRadius = 0.0f;
		float AttackDamage = AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].AttackDamage;

		TArray<FHitResult> HitResults;

		switch (AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].TraceShape)
		{
		case EAttackTraceShape::Sphere:

			AttackTraceRadius = AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].TraceSize.X;

			UKismetSystemLibrary::SphereTraceMulti (GetWorld(),
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
				0.5f);

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

				//

				//IAbilitySystemInterface* OutASCInterface = Cast <IAbilitySystemInterface>(OutCharacter);
				//UAbilitySystemComponent* OutASC = nullptr;
				//if (OutASCInterface != nullptr)
				//{
				//	OutASC = OutASCInterface->GetAbilitySystemComponent();
				//}

				//if (OutASC != nullptr)
				//{
				//	UGameplayEffect* GameplayEffect = AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].AttackDamageGameplayEffect->GetDefaultObject<UGameplayEffect>();
				//	FGameplayEffectContextHandle GameplayEffectContextHandle = OutASC->MakeEffectContext();

				//	OutASC->ApplyGameplayEffectToSelf(GameplayEffect, 0.0f, GameplayEffectContextHandle);
				//}
			}
			
			break;

		case EAttackTraceShape::None:
		default:

			break;
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
