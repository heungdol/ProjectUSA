// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/AT/USAAbilityTask.h"
#include "AT_PlayAnimMontages.generated.h"


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

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage Info")
	FPlayAnimMontageSectionDetail StartAnimMontageSectionDetail;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anim Montage Info")
	TArray <FPlayAnimMontageSectionDetail> MiddleAnimMontageSectionDetails;

	//

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

	FTimerHandle CallSectionTimerHandle;
	void OnSectionTimerHandleEnd();

	//UFUNCTION(Server, Reliable, WithValidation)
	//void ServerRPC_Test ();

	//UFUNCTION(NetMulticast, Reliable)
	//void MulticastRPC_Test();

protected:
	/** Checks if the ability is playing a montage and stops that montage, returns true if a montage was stopped, false if not. */
	bool StopPlayingMontage();

	const FPlayAnimMontageData* PlayAnimMontageData;
	int CurrentPlayAnimMontageIndex = 0;

	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;
	FDelegateHandle InterruptedHandle;
};
