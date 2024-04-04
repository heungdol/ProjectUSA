// Fill out your copyright notice in the Description page of Project Settings.


#include "AT_SetCharacterVelocityForPeriod.h"

#include "AbilitySystemComponent.h"

#include "GameFramework/Character.h"


UAT_SetCharacterVelocityForPeriod* UAT_SetCharacterVelocityForPeriod::GetNewAbilityTask(UGameplayAbility* OwningAbility, FVector InVelocity, bool InOverrideXY, bool InOverrideZ, float InPeriod)
{
	UAT_SetCharacterVelocityForPeriod* MyObj = NewAbilityTask<UAT_SetCharacterVelocityForPeriod>(OwningAbility);

	//MyObj->MyCharacter = InCharacter;
	MyObj->LaunchVelocity = InVelocity;
	MyObj->bXYOverride = InOverrideXY;
	MyObj->bZOverride = InOverrideZ;
	MyObj->Period = InPeriod;
	
	MyObj->bTickingTask = true;

	return MyObj;
}

void UAT_SetCharacterVelocityForPeriod::Activate()
{
	Super::Activate();

	//bIsSettingVelocity = true;

	StartTime = GetWorld()->GetTimeSeconds();
	EndTime = StartTime + Period;
	
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->GenericLocalCancelCallbacks.AddDynamic(this, &UAT_SetCharacterVelocityForPeriod::OnCancelCallback);
	}
}

void UAT_SetCharacterVelocityForPeriod::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	//if (bIsSettingVelocity == false)
	//{
	//	return;
	//}

	if (Ability->GetCurrentActorInfo() == nullptr)
	{
		return;
	}

	ACharacter* MyCharacter = Cast <ACharacter>(Ability->GetCurrentActorInfo()->AvatarActor);
	if (MyCharacter == nullptr)
	{
		return;
	}

	MyCharacter->LaunchCharacter(LaunchVelocity, bXYOverride, bZOverride);

	if (EndTime < GetWorld()->GetTimeSeconds())
	{
		//bIsSettingVelocity = false;
		
		OnFinished.Broadcast();
		EndTask();
	}
}

void UAT_SetCharacterVelocityForPeriod::OnCancelCallback()
{
	//bIsSettingVelocity = false;
	
	OnFinished.Broadcast();
	EndTask();
}
