// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTT/BTT_ActivateGA.h"

#include "AIModule/Classes/AIController.h"


EBTNodeResult::Type UBTT_ActivateGA::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (InGameplayAbilityClass == nullptr)
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

	AbilitySpec = ASC->BuildAbilitySpecFromClass(InGameplayAbilityClass, InLevel, InInputID);
	FGameplayAbilitySpecHandle GameplayAbilitySpecHandle = ASC->GiveAbility(AbilitySpec);

	ASC->TryActivateAbility(GameplayAbilitySpecHandle);

	bIsSetDelegates = false;

	return EBTNodeResult::InProgress;
}

void UBTT_ActivateGA::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (AbilitySpec.GetPrimaryInstance() == nullptr)
	{
		return;
	}

	if (bIsSetDelegates == false)
	{
		AbilitySpec.GetPrimaryInstance()->OnGameplayAbilityCancelled.AddUObject(this, &UBTT_ActivateGA::OnAbilityCancelCallback);
		AbilitySpec.GetPrimaryInstance()->OnGameplayAbilityEnded.AddUObject(this, &UBTT_ActivateGA::OnAbilityEndCallback);

		UE_LOG(LogTemp, Log, TEXT("Set Delegates"));

		bIsSetDelegates = true;
	}

}

void UBTT_ActivateGA::OnAbilityCancelCallback()
{
	UE_LOG(LogTemp, Log, TEXT("BT Task Cancelled"));

	UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
	FinishLatentTask(*OwnerComp, EBTNodeResult::Aborted);

}

void UBTT_ActivateGA::OnAbilityEndCallback(UGameplayAbility* InGA)
{
	UE_LOG(LogTemp, Log, TEXT("BT Task Finished"));
	
	UBehaviorTreeComponent* OwnerComp = Cast<UBehaviorTreeComponent>(GetOuter());
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);

}