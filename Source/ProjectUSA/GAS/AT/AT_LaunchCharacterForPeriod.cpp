// Fill out your copyright notice in the Description page of Project Settings.


#include "AT_LaunchCharacterForPeriod.h"

#include "AbilitySystemComponent.h"

#include "GameFramework/Character.h"


UAT_LaunchCharacterForPeriod* UAT_LaunchCharacterForPeriod::GetNewAbilityTask(UGameplayAbility* OwningAbility, FVector InVelocity, bool InOverrideXY, bool InOverrideZ, float InPeriod)
{
	UAT_LaunchCharacterForPeriod* MyObj = NewAbilityTask<UAT_LaunchCharacterForPeriod>(OwningAbility);

	//MyObj->MyCharacter = InCharacter;
	MyObj->LaunchVelocity = InVelocity;
	MyObj->bXYOverride = InOverrideXY;
	MyObj->bZOverride = InOverrideZ;
	MyObj->Period = InPeriod;
	
	MyObj->bTickingTask = true;

	return MyObj;
}

void UAT_LaunchCharacterForPeriod::Activate()
{
	Super::Activate();

	StartTime = GetWorld()->GetTimeSeconds();
	EndTime = StartTime + Period;
	
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->GenericLocalCancelCallbacks.AddDynamic(this, &UAT_LaunchCharacterForPeriod::OnCancelCallback);
	}
}

void UAT_LaunchCharacterForPeriod::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

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

	if (Period > SMALL_NUMBER
		&& EndTime < GetWorld()->GetTimeSeconds())
	{
		OnFinished.Broadcast();
		EndTask();
	}
}

void UAT_LaunchCharacterForPeriod::OnCancelCallback()
{
	OnFinished.Broadcast();
	EndTask();
}
