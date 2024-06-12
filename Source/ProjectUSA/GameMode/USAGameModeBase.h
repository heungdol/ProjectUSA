// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "USAGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API AUSAGameModeBase : public AGameMode
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Game Mode Info")
	int32 CurrentPlayerStackCount = 0;
	
public:
	//virtual void RestartPlayer(AController* NewPlayer) override;

	//virtual void RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot) override;

	//virtual void RestartPlayerAtTransform(AController* NewPlayer, const FTransform& SpawnTransform) override;

	//virtual AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName = TEXT("")) override;

	// Restart->FindPlayerStart->ChoosePlayerStart->RestartPlayerAtPlayerStart
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;

	virtual void InitStartSpot_Implementation(AActor* StartSpot, AController* NewPlayer) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	//virtual bool ShouldSpawnAtStartSpot(AController* Player) override;

	//virtual void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation) override;


public:
	UFUNCTION(BlueprintCallable)
	void SetAllPlayerControllerInput (bool InActive);

	UFUNCTION(BlueprintCallable)
	void SetBossUSACharacter (class AUSACharacterBase* InCharacter);

	//

	UFUNCTION(BlueprintCallable)
	void UpdateBossName(FName InName);

	UFUNCTION(BlueprintCallable)
	void UpdateBossHealthRatio(float InRatio, float InMax, float InCurrent);

	//
	
	UFUNCTION(BlueprintCallable)
	void PlayLevelSequenceToAllPlayer(class ALevelSequenceActor* InLevelSequnce);

	//

	//UFUNCTION(BlueprintCallable)
	//virtual void RestartUSAPlayer(class AUSAPlayerController* NewPlayer);

	void UpdatePlayerControllerCheckpoint(class AUSAPlayerController* InPlayer, int32 InCheckpointIndex);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Boss Character Info");
	TObjectPtr<class AUSACharacterBase> BossUSACharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Player Checkpoint Info");
	TMap<TObjectPtr<class AUSAPlayerController>, int32> PlayerControllerCheckpointMapList;

	//UFUNCTION(BlueprintImplementableEvent)
	//void K2_RestartUSAPlayer(class AUSAPlayerController* NewPlayer, class AUSACharacterPlayer* PlayerCharacter);
};
