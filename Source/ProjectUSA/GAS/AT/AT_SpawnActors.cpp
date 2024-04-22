// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AT/AT_SpawnActors.h"

void UAT_SpawnActors::Activate()
{
	Super::Activate();
}

UAT_SpawnActors* UAT_SpawnActors::GetNewAbilityTask_SpawnActors(UGameplayAbility* OwningAbility, float Time)
{
	UAT_SpawnActors* MyObj = NewAbilityTask<UAT_SpawnActors>(OwningAbility);

	return MyObj;
}
