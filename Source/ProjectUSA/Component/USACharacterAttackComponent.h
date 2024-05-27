// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Struct/USAStructs.h"

#include "USACharacterAttackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTUSA_API UUSACharacterAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UUSACharacterAttackComponent();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	void RegisterUSAAttackInfo(const FAttackTraceInfo& InAttack, 
		const FVector& InLocation,
		const FVector& InForward, const FVector& InRight, const FVector& InUp);

protected:

	TArray <FAttackTraceSceneInfo> CurrentAttackTraceSceneInfos;
};
