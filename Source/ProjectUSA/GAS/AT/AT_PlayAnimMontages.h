// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/AT/USAAbilityTask.h"

#include "Struct/USAStructs.h"

#include "AT_PlayAnimMontages.generated.h"


//USTRUCT(BlueprintType)
//struct FPlayAnimMontageSectionDetail
//{
//	GENERATED_BODY()
//
//public:
//	//float SectionPlayTime = -1.0f;
//	
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage Section Detail")
//	FGameplayTag SectionGameplayTagAdded;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage Section Detail")
//	FGameplayTag SectionGameplayTagRemoved;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage Section Detail")
//	FName SectionName = NAME_None;
//};
//



// ========================================================================================


/**
 * 
 */
UCLASS()
class PROJECTUSA_API UAT_PlayAnimMontages : public UUSAAbilityTask
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintAssignable)
	FUSAATSimpleDelegate	OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FUSAATSimpleDelegate	OnBlendOut;

	UPROPERTY(BlueprintAssignable)
	FUSAATSimpleDelegate	OnInterrupted;

	UPROPERTY(BlueprintAssignable)
	FUSAATSimpleDelegate	OnCancelled;

	
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (/*DisplayName = "PlayMontageAndWait",*/
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_PlayAnimMontages* GetNewAbilityTask_PlayAnimMontages
	(UGameplayAbility* OwningAbility, const FPlayAnimMontageData& AnimMontageData);

	virtual void Activate() override;

	virtual void SimpleEndAbilityTask() override;
	virtual void SimpleCancelAbilityTask() override;

	UFUNCTION()
	void OnAnimSectionGameplayTagAdded(const FGameplayTag InTag, int32 NewCount);

	UFUNCTION()
	void OnAnimSectionGameplayTagRemoved(const FGameplayTag InTag, int32 NewCount);


	//FTimerHandle CallSectionTimerHandle;
	//void OnSectionTimerHandleEnd();

protected:
	virtual void OnDestroy(bool AbilityIsEnding) override;
	
	/** Checks if the ability is playing a montage and stops that montage, returns true if a montage was stopped, false if not. */
	bool StopPlayingMontage();

	const FPlayAnimMontageData* PlayAnimMontageData;
	//int CurrentPlayAnimMontageIndex = 0;

	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;
	FDelegateHandle InterruptedHandle;

	TMap<FGameplayTag, FDelegateHandle> DelegateHandles;
};
