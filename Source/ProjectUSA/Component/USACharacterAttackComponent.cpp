// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/USACharacterAttackComponent.h"

#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
UUSACharacterAttackComponent::UUSACharacterAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UUSACharacterAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UUSACharacterAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (UKismetSystemLibrary::IsServer(GetWorld()) == false
		&& UKismetSystemLibrary::IsStandalone(GetWorld()) == false)
	{
		return;
	}


	for (int i = CurrentAttackTraceSceneInfos.Num() - 1; i >= 0; i--)
	{
		CurrentAttackTraceSceneInfos[i].DoAttackTrace(GetWorld(), DeltaTime);

		if (CurrentAttackTraceSceneInfos[i].GetIsAttackEnded(GetWorld()) == true)
		{
			CurrentAttackTraceSceneInfos.RemoveAt(i);
		}
	}
}

void UUSACharacterAttackComponent::RegisterUSAAttackInfo(const FAttackTraceInfo& InAttack, 
	const FVector& InLocation,
	const FVector& InForward, const FVector& InRight, const FVector& InUp)
{
	if (UKismetSystemLibrary::IsServer(GetWorld()) == false
		&& UKismetSystemLibrary::IsStandalone(GetWorld()) == false)
	{
		return;
	}

	FAttackTraceSceneInfo NewAttackTraceSceneInfo(GetWorld(), InAttack, GetOwner(), InLocation, InForward, InRight, InUp);
	
	CurrentAttackTraceSceneInfos.Add(NewAttackTraceSceneInfo);
}

