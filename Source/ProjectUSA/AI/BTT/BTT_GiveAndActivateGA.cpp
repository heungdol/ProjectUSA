// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT/BTT_GiveAndActivateGA.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

#include "AIModule/Classes/AIController.h"

//#include "GAS/GA/USAGameplayAbility.h"
//
//#include "ProjectUSA.h"


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
	//FGameplayAbilitySpecHandle GameplayAbilitySpecHandle = ASC->GiveAbility(AbilitySpec);

	//AbilitySpec.GetPrimaryInstance()->OnGameplayAbilityCancelled.AddUObject(this, &UBTT_GiveAndActivateGA::OnAbilityCancelCallback);
	//AbilitySpec.GetPrimaryInstance()->OnGameplayAbilityEnded.AddUObject(this, &UBTT_GiveAndActivateGA::OnAbilityEndCallback);

	//ASC->TryActivateAbility(GameplayAbilitySpecHandle);

	ASC->GiveAbilityAndActivateOnce(AbilitySpec);

	return EBTNodeResult::Succeeded;
}

void UBTT_GiveAndActivateGA::OnAbilityCancelCallback()
{
	//if (ASC != nullptr)
	//{
	//	ASC->ClearAbility(GameplayAbilitySpecHandle);
	//}

	//UE_LOG(LogTemp, Log, TEXT("BT Task Cancelled"));
	UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
	FinishLatentTask(*OwnerComp, EBTNodeResult::Aborted);

}

void UBTT_GiveAndActivateGA::OnAbilityEndCallback(UGameplayAbility* InGA)
{
	//if (ASC != nullptr)
	//{
	//	ASC->ClearAbility(GameplayAbilitySpecHandle);
	//}

	//UE_LOG(LogTemp, Log, TEXT("BT Task Finished"));
	UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);

}

void UBTT_GiveAndActivateGA::OnAbilityEndStateCallback(FName InName)
{
	UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);

	//UE_LOG(LogTemp, Log, TEXT("BT Task Finished"));
}
