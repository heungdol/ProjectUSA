// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_PlayAnimMontages.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSimpleDeletage);

// ========================================================================================

USTRUCT(BlueprintType)
struct FPlayAnimMontageSectionDetail
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage Section Detail")
	float SectionPlayTime = -1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage Section Detail")
	FName SectionName = NAME_None;
};


USTRUCT(BlueprintType)
struct FPlayAnimMontageData
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage Info")
	TObjectPtr <UAnimMontage> AnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage Info")
	float AnimMontageRate = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage Info")
	FPlayAnimMontageSectionDetail StartAnimMontageSectionDetail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage Info")
	TArray <FPlayAnimMontageSectionDetail> MiddleAnimMontageSectionDetails;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage Info")
	bool bHasEndSection = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage Info")
	FPlayAnimMontageSectionDetail EndAnimMontageSectionDetail;

};


// ========================================================================================


/**
 * 
 */
UCLASS()
class PROJECTUSA_API UAT_PlayAnimMontages : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	//UAT_PlayAnimMontages(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable)
	FSimpleDeletage	OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FSimpleDeletage	OnBlendOut;

	UPROPERTY(BlueprintAssignable)
	FSimpleDeletage	OnInterrupted;

	UPROPERTY(BlueprintAssignable)
	FSimpleDeletage	OnCancelled;

	//UFUNCTION()
	//void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	//UFUNCTION()
	//void OnMontageInterrupted();

	//UFUNCTION()
	//void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "PlayMontageAndWait",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	//static UAT_PlayAnimMontages* GetNewAbilityTask(UGameplayAbility* OwningAbility,
	//	FName TaskInstanceName, UAnimMontage* MontageToPlay, float Rate = 1.f, FName StartSection = NAME_None, bool bStopWhenAbilityEnds = true, float AnimRootMotionTranslationScale = 1.f, float StartTimeSeconds = 0.f);

	static UAT_PlayAnimMontages* GetNewAbilityTask
	(UGameplayAbility* OwningAbility, const FPlayAnimMontageData& AnimMontageData);

	virtual void Activate() override;

	/** Called when the ability is asked to cancel from an outside node. What this means depends on the individual task. By default, this does nothing other than ending the task. */
	virtual void ExternalCancel() override;

	//virtual FString GetDebugString() const override;

	UFUNCTION()
	void OnEndTaskCallback();

	UFUNCTION()
	void OnCancelTaskCallback ();

	FTimerHandle CallSectionTimerHandle;
	void OnSectionTimerHandleEnd();

protected:

	//virtual void OnDestroy(bool AbilityEnded) override;

	/** Checks if the ability is playing a montage and stops that montage, returns true if a montage was stopped, false if not. */
	bool StopPlayingMontage();

	const FPlayAnimMontageData* PlayAnimMontageData;
	int CurrentPlayAnimMontageIndex = 0;

	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;
	FDelegateHandle InterruptedHandle;

	//UPROPERTY()
	//TObjectPtr<UAnimMontage> MontageToPlay;

	//UPROPERTY()
	//float Rate;

	//UPROPERTY()
	//FName StartSection;

	//UPROPERTY()
	//float AnimRootMotionTranslationScale;

	//UPROPERTY()
	//float StartTimeSeconds;

	//UPROPERTY()
	//bool bStopWhenAbilityEnds;
};
