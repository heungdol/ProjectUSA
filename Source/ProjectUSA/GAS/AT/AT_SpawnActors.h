// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/AT/USAAbilityTask.h"
#include "AT_SpawnActors.generated.h"

USTRUCT(BlueprintType)
struct FSpawnActorDetail
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Actor Detail")
	TSubclassOf<AActor> DesiredActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Actor Detail")
	float SpawnTime = -1.0f;

};


USTRUCT(BlueprintType)
struct FSpawnActorData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Actor Data")
	TArray <FSpawnActorDetail> SpawnActorDetails;

};


// ========================================================================================


/**
 * 
 */
UCLASS()
class PROJECTUSA_API UAT_SpawnActors : public UUSAAbilityTask
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_SpawnActors* GetNewAbilityTask_SpawnActors(UGameplayAbility* OwningAbility, const FSpawnActorData& InSpawnActorData);

	virtual void Activate() override;

	void SpawnActorAndSetNextTimer();

public:
	const FSpawnActorData* SpawnActorData;

	FTimerHandle SpawnActorTimerHandle;

	float PrevSpawnActorTime = 0.0f;
	int CurrentSpwanActorIndex = 0;
};
