// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "USAGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API AUSAGameStateBase : public AGameState
{
	GENERATED_BODY()

protected:
	//UPROPERTY()
	//class ULevelSequencePlayer* SequencePlayer;

	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	void ShowHideBossHPBar(bool bIsShowing);

	//UFUNCTION(BlueprintCallable)
	//void HideBossHPBar();

	UFUNCTION(BlueprintCallable)
	void UpdateBossHPBarRatio(float InRatio);

	//

	//UFUNCTION(BlueprintCallable)
	void SetPlayerControllerInput (bool InActive);

	//

	//UFUNCTION(BlueprintCallable)
	void PlayLevelSequence (class ALevelSequenceActor* InLevelSequence);

	//UFUNCTION()
	//void UpdateBossHPBar(float InCurrent, float InMax);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Player Controller")
	TObjectPtr<class AUSACharacterBase> LocalUSACharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Player Controller")
	TObjectPtr<class AUSAPlayerController> LocalUSAPlayerController;
	
	//

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetPlayerControllerInput(bool InActive);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SetPlayerControllerInput(bool InActive);

	UFUNCTION(BlueprintCallable)
	void SetPlayerControllerInputFinal(bool InActive);

	//

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayLevelSequence(class ALevelSequenceActor* InLevelSequence);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_PlayLevelSequence(class ALevelSequenceActor* InLevelSequence);

	UFUNCTION(BlueprintCallable)
	void PlayLevelSequenceFinal(class ALevelSequenceActor* InLevelSequence);

	//

	UFUNCTION()
	void OnRep_bIsShowingBossHPBar();
	
	UPROPERTY(ReplicatedUsing=OnRep_bIsShowingBossHPBar, VisibleAnywhere, BlueprintReadOnly, Category = "USA Boss Info")
	bool bIsShowingBossHPBar = false;

	UFUNCTION()
	void OnRep_CurrentBossHealthRatio();

	UPROPERTY(ReplicatedUsing = OnRep_CurrentBossHealthRatio, VisibleAnywhere, BlueprintReadOnly, Category = "USA Boss Info")
	float CurrentBossHealthRatio = 0.0f;

	//

	UFUNCTION(BlueprintImplementableEvent)
	void K2_ShowHideBossHPBar(bool InShowing);

	//UFUNCTION(BlueprintImplementableEvent)
	//void K2_HideBossHPBar();

	UFUNCTION(BlueprintImplementableEvent)
	void K2_UpdateBossHPBarRatio(float InRatio);

	//

	UFUNCTION(BlueprintImplementableEvent)
	void K2_PlayLevelSequence(ALevelSequenceActor* InLevelSequence);

	//

	UFUNCTION(BlueprintCallable)
	void PlayUserWidgetAnimationP_Panel (bool bIsShowing, bool bIsRaw);
};
