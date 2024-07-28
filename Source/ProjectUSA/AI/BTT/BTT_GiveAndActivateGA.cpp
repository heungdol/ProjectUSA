// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTT/BTT_GiveAndActivateGA.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

#include "AIModule/Classes/AIController.h"


UBTT_GiveAndActivateGA::UBTT_GiveAndActivateGA()
{
	NodeName = TEXT("Give And Activate Gameplay Ability Once");
}

EBTNodeResult::Type UBTT_GiveAndActivateGA::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (InGameplayAbility == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	if (OwnerComp.GetAIOwner()->GetPawn() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	IAbilitySystemInterface* AbilitySystemInterface = Cast <IAbilitySystemInterface>(OwnerComp.GetAIOwner()->GetPawn());
	if (AbilitySystemInterface == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInterface->GetAbilitySystemComponent();
	if (AbilitySystemComponent == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ASC = AbilitySystemComponent;

	FGameplayAbilitySpec AbilitySpec = ASC->BuildAbilitySpecFromClass(InGameplayAbility, InLevel, InInputID);

	ASC->GiveAbilityAndActivateOnce(AbilitySpec);

	return EBTNodeResult::Succeeded;
}

void UBTT_GiveAndActivateGA::OnAbilityCancelCallback()
{
	UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
	FinishLatentTask(*OwnerComp, EBTNodeResult::Aborted);

}

void UBTT_GiveAndActivateGA::OnAbilityEndCallback(UGameplayAbility* InGA)
{
	UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);

}

void UBTT_GiveAndActivateGA::OnAbilityEndStateCallback(FName InName)
{
	UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);

}
