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

	UFUNCTION(BlueprintCallable)
	void UpdateBossName(FName InName);

	UFUNCTION(BlueprintCallable)
	void UpdateBossHealthRatio(float InRatio);

	//

	//UFUNCTION(BlueprintCallable)
	//void SetPlayerControllerInput (bool InActive);

	//

	//UFUNCTION(BlueprintCallable)
	void PlayLevelSequenceMulti (class ALevelSequenceActor* InLevelSequence);

	UFUNCTION(BlueprintCallable)
	void PlayLevelSequenceSingle(class ALevelSequenceActor* InLevelSequence);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Player Controller")
	TObjectPtr<class AUSACharacterPlayer> LocalUSACharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Player Controller")
	TObjectPtr<class AUSAPlayerController> LocalUSAPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Player Controller")
	TObjectPtr<class AUSAHUD> LocalUSAHUD;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "USA Level Sequence Info")
	TObjectPtr<class ALevelSequenceActor> MultiLevelSeqeunceActor;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "USA Level Sequence Info")
	TObjectPtr<class ALevelSequenceActor> SingleLevelSeqeunceActor;

	//

	//UFUNCTION(Server, Reliable)
	//void ServerRPC_SetPlayerControllerInput(bool InActive);

	//UFUNCTION(NetMulticast, Reliable)
	//void MulticastRPC_SetPlayerControllerInput(bool InActive);

	//UFUNCTION(BlueprintCallable)
	//void SetPlayerControllerInputFinal(bool InActive);

	//

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayLevelSequence(class ALevelSequenceActor* InLevelSequence);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_PlayLevelSequence(class ALevelSequenceActor* InLevelSequence);

	UFUNCTION(BlueprintCallable)
	void PlayLevelSequenceCore(class ALevelSequenceActor* InLevelSequence);

	//

	UFUNCTION()
	void OnRep_bIsShowingBossHPBar();
	
	UPROPERTY(ReplicatedUsing=OnRep_bIsShowingBossHPBar, VisibleAnywhere, BlueprintReadOnly, Category = "USA Boss Info")
	bool bIsShowingBossHPBar = false;

	UFUNCTION()
	void OnRep_CurrentBossName();

	UPROPERTY(ReplicatedUsing = OnRep_CurrentBossName, VisibleAnywhere, BlueprintReadOnly, Category = "USA Boss Info")
	FName CurrentBossName = TEXT ("");


	UFUNCTION()
	void OnRep_CurrentBossHealthRatio();

	UPROPERTY(ReplicatedUsing = OnRep_CurrentBossHealthRatio, VisibleAnywhere, BlueprintReadOnly, Category = "USA Boss Info")
	float CurrentBossHealthRatio = 0.0f;

	//

	UFUNCTION(BlueprintImplementableEvent)
	void K2_ShowHideBossHPBar(bool InShowing);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_UpdateBossName(FName InName);
	
	UFUNCTION(BlueprintImplementableEvent)
	void K2_UpdateBossHealthRatio(float InRatio);

	//

	UFUNCTION(BlueprintImplementableEvent)
	void K2_PlayLevelSequenceMulti(ALevelSequenceActor* InLevelSequence);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_PlayLevelSequenceSingle(ALevelSequenceActor* InLevelSequence);

	//

	UFUNCTION(BlueprintCallable)
	void PlayUserWidgetAnimationP_Panel (bool bIsShowing, bool bIsRaw);
};
