// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AT/AT_TraceAttack.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Debug/DebugDrawComponent.h"

#include "GameFramework/Character.h"

#include "Engine/DamageEvents.h"
#include "GameFramework/DamageType.h"



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

	
	while (CurrentAttackTraceIndex < AttackTraceData->AttackTraceInfos.Num()
		&& PrevAttackTraceTime + SMALL_NUMBER >= NextSpawnTime)
	{
		FVector AttackTraceLocation = GetAvatarActor()->GetActorLocation();
		AttackTraceLocation += GetAvatarActor()->GetActorForwardVector() * AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].OffsetTraceLocation.X;
		AttackTraceLocation += GetAvatarActor()->GetActorRightVector() * AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].OffsetTraceLocation.Y;
		AttackTraceLocation += GetAvatarActor()->GetActorUpVector() * AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].OffsetTraceLocation.Z;

		float AttackTraceRadius = 0.0f;
		float AttackDamage = AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].AttackDamage;

		FHitResult HitResult;

		TArray<AActor*> OutActors;

		switch (AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].TraceShape)
		{
		case EAttackTraceShape::Sphere:

			AttackTraceRadius = AttackTraceData->AttackTraceInfos[CurrentAttackTraceIndex].TraceSize.X;

			UKismetSystemLibrary::SphereOverlapActors
			(GetWorld(),
				AttackTraceLocation,
				AttackTraceRadius,
				TraceObjectTyps,
				nullptr,
				IgnoreActors,
				OutActors);

			DrawDebugSphere(GetWorld(), AttackTraceLocation, AttackTraceRadius, 16, FColor::Emerald, false, 0.5f, 0U, 2.0f);
			
			for (AActor* OutActor : OutActors)
			{
				ACharacter* OutCharacter = Cast <ACharacter>(OutActor);

				if (OutCharacter == nullptr)
				{
					continue;
				}

				FDamageEvent DamageEvent;

				OutCharacter->TakeDamage(AttackDamage, DamageEvent, MyCharacter->GetController(), MyCharacter);
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
