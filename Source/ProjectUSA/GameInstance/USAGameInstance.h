// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Engine/DamageEvents.h"

#include "Item/USAItemBase.h"

#include "USAGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UUSAGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	virtual void OnStart() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA DamageType")
	TSubclassOf<class UDamageType> USADamageType_Explosion;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA DamageType")
	TSubclassOf<class UDamageType> USADamageType_Grab;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Item")
	TArray<TSubclassOf<class AUSAItemBase>> USAItemOrder;

public:
	FORCEINLINE TSubclassOf<class UDamageType> GetUSADamageType_Explosion() {return USADamageType_Explosion;}

	FORCEINLINE TSubclassOf<class UDamageType> GetUSADamageType_Grab() { return USADamageType_Grab; }

	FORCEINLINE TArray<TSubclassOf<class AUSAItemBase>> GetUSAItemOrder() { return USAItemOrder; }

};
