// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/AT/AT_SpawnActors.h"

#include "GameFramework/Character.h"

#include "GameFramework/Actor.h"



UAT_SpawnActors* UAT_SpawnActors::GetNewAbilityTask_SpawnActors(UGameplayAbility* OwningAbility, const FSpawnActorData& InSpawnActorData)
{
	UAT_SpawnActors* MyObj = NewAbilityTask<UAT_SpawnActors>(OwningAbility);

	MyObj->SpawnActorData = &InSpawnActorData;

	MyObj->bIsCancelable = true;

	return MyObj;
}


void UAT_SpawnActors::Activate()
{
	Super::Activate();

	SpawnActorAndSetNextTimer();
}


void UAT_SpawnActors::SpawnActorAndSetNextTimer()
{
	if (SpawnActorData == nullptr)
	{
		return;
	}

	if (CurrentSpwanActorIndex >= SpawnActorData->SpawnActorDetails.Num())
	{
		return;
	}

	float NextSpawnTime = SpawnActorData->SpawnActorDetails[CurrentSpwanActorIndex].SpawnTime;

	while (CurrentSpwanActorIndex < SpawnActorData->SpawnActorDetails.Num()
		&& PrevSpawnActorTime + SMALL_NUMBER >= NextSpawnTime)
	{
		if (SpawnActorData->SpawnActorDetails[CurrentSpwanActorIndex].DesiredActorClass)
		{
			FVector SpawnLocation = GetAvatarActor()->GetActorLocation();
			FRotator SpawnRotation = GetAvatarActor()->GetActorRotation();

			AActor* NewActor = GetWorld()->SpawnActor<AActor>
				(SpawnActorData->SpawnActorDetails[CurrentSpwanActorIndex].DesiredActorClass, SpawnLocation, SpawnRotation);

			if (NewActor)
			{
				// 스폰된 액터 처리
			}
			else
			{
				// 스폰에 실패한 경우 처리
			}
		}

		PrevSpawnActorTime = FMath::Max(PrevSpawnActorTime, NextSpawnTime);

		CurrentSpwanActorIndex += 1;

		if (CurrentSpwanActorIndex < SpawnActorData->SpawnActorDetails.Num())
		{
			NextSpawnTime = SpawnActorData->SpawnActorDetails[CurrentSpwanActorIndex].SpawnTime;
		}
	}

	if (CurrentSpwanActorIndex < SpawnActorData->SpawnActorDetails.Num())
	{
		float TimeDelay = NextSpawnTime - PrevSpawnActorTime;
		PrevSpawnActorTime = NextSpawnTime;

		GetWorld()->GetTimerManager().SetTimer(SpawnActorTimerHandle, this, &UAT_SpawnActors::SpawnActorAndSetNextTimer, TimeDelay, false);
	}
}