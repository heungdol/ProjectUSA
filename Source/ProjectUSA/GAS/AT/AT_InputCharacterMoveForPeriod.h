// Fill out your copyright notice in the Description page of Project Settings.

//#pragma once
//
//#include "CoreMinimal.h"
//#include "GAS/AT/USAAbilityTask.h"
//#include "AT_InputCharacterMoveForPeriod.generated.h"
//
///**
// * 
// */
//UCLASS()
//class PROJECTUSA_API UAT_InputCharacterMoveForPeriod : public UUSAAbilityTask
//{
//	GENERATED_BODY()
//
//public:
//	/** Spawn new Actor on the network authority (server) */
//	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"), Category = "Ability|Tasks")
//	static UAT_InputCharacterMoveForPeriod* GetNewAbilityTask_InputCharacterMoveForPeriod
//	(UGameplayAbility* OwningAbility, class ACharacter* InCharacter, FVector InDirection, float InPeriod);
//	
//	virtual void Activate() override;
//	virtual void TickTask(float DeltaTime) override;
//
//public:
//	//UFUNCTION()
//	//void OnCancelTaskCallback();
//
//	//UFUNCTION()
//	//void OnEndTaskCallback();
//
//public:
//	TObjectPtr <class ACharacter> MyCharacter;
//	FVector Direction;
//	float Period;
//
//	float EndTime;
//};
