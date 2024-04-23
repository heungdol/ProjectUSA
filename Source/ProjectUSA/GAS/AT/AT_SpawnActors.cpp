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
	ACharacter* MyCharacter = Cast <ACharacter>(GetAvatarActor());
	
	if (MyCharacter == nullptr)
	{
		return;
	}

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
		TSubclassOf<AActor> ActorClass = SpawnActorData->SpawnActorDetails[CurrentSpwanActorIndex].DesiredActorClass;

		if (ActorClass)
		{
			//bool bIsActorReplicated = false;
			//bool bIsOkay = false;

			//if (Cast<AActor>(ActorClass))
			//{
			//	bIsActorReplicated = Cast<AActor>(ActorClass)->GetIsReplicated();
			//}

			//if (bIsActorReplicated == true && MyCharacter->HasAuthority())
			//{
			//	bIsOkay = true;
			//}

			//if (bIsActorReplicated == false)
			//{
			//	bIsOkay = true;
			//}

			//if (bIsOkay == false)
			//{
			//	continue;
			//}

			FVector SpawnLocation = GetAvatarActor()->GetActorLocation();
			FRotator SpawnRotation = GetAvatarActor()->GetActorRotation();

			AActor* NewActor = GetWorld()->SpawnActor<AActor> (ActorClass, SpawnLocation, SpawnRotation);

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