// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USACharacterBase.h"

#include "Interface/USAPickableInterface.h"

#include "USACharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API AUSACharacterPlayer : public AUSACharacterBase
{
	GENERATED_BODY()
	
public:
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Camera Info: Player Only")
	TSubclassOf <class AUSATargetingCameraActor> TargetingCameraActorClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Camera Info: Player Only")
	TObjectPtr <class AUSATargetingCameraActor> TargetingCameraActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Camera Info: Player Only")
	TObjectPtr <class AUSAPlacedCameraActor> CurrentPlacedCameraActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Camera Info: Player Only")
	TObjectPtr <class AUSAPlacedCameraActor> PrevPlacedCameraActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Player Controller")
	TObjectPtr <class APlayerController> LocalPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Player Controller")
	TObjectPtr <class AUSAPlayerController> LocalUSAPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Player Controller")
	TObjectPtr<class AUSAHUD> LocalUSAHUD;


	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Camera Shake")
	TMap <TSubclassOf<class UDamageType>, TSubclassOf<class UCameraShakeBase>> HitSuccessCameraShakes;


public:
	AUSACharacterPlayer();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Move(const struct FInputActionValue& Value) override;

	virtual void PossessedBy(AController* NewController) override;
	
	virtual void OnRep_PlayerState() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void InitPlayerController();
	void InitTargetingCameraActor();

	//

	int8 bIsTargetingCamera : 1;
	virtual void LookTarget(const struct FInputActionValue& Value) override;
	
	void StartTargeting();
	void KeepTargeting();
	void ChangeTargeting();
	void FinishTargeting();

	//

	//UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnStartTargetingCamera", ScriptName = "OnStartTargetingCamera"))
	//void K2_OnStartTargeting();

	//UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnKeepTargetingCamera", ScriptName = "OnStartTargetingCamera"))
	//void K2_OnKeepTargeting();
	
	//UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnFinishTargetingCamera", ScriptName = "OnFinishTargetingCamera"))
	//void K2_OnFinishTargeting();

	//

	void StartPlacedCamera(AUSAPlacedCameraActor* InActor);
	
	void FinishPlacedCamera(AUSAPlacedCameraActor* InActor);
	
	//

	//UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnStartPlacedCamera", ScriptName = "OnStartPlacedCamera"))
	//void K2_OnStartPlacedCamera();


	//

	void ManageAllCamera();

	//UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnFinishPlacedCamera", ScriptName = "OnFinishPlacedCamera"))
	//void K2_OnFinishPlacedCamera();


	//virtual void DoDrop(const struct FInputActionValue& Value) override;

	//

	virtual void StartCameraShake_HitSuccess (TSubclassOf<class UDamageType> DamageType) override;

	//

	UFUNCTION(BlueprintCallable)
	void SetPlayerControllerInput(bool bIsEnable);

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetPlayerControllerInput(bool bIsEnable);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SetPlayerControllerInput(bool bIsEnable);

	UFUNCTION(BlueprintCallable)
	void SetLocalPlayerControllerInput(bool bIsEnable);

	//

	UFUNCTION(BlueprintCallable)
	void PlayUserWidgetAnimation_Panel(bool IsShowing, bool IsRaw);

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayUserWidgetAnimation_Panel(bool IsShowing, bool IsRaw);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_PlayUserWidgetAnimation_Panel(bool IsShowing, bool IsRaw);

	//

protected:
	virtual void DownUSACharacter() override;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Player Restart Info")
	float RestartDelay = 3.0f;

	UPROPERTY()
	FTimerHandle RestartTimerHandle;
	
	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnUSARestart();

	UFUNCTION(BlueprintCallable)
	void RestartUSAPlayer();

	//

	virtual void Look(const struct FInputActionValue& Value) override;
	
	virtual void SetupGAS() override;
	virtual void OnRep_ASC() override;

	void UpdateCurrentTargetableActor() override;
	void UpdateCurrentTargetableActor_Instant() override;

	void SetCurrentTargetableActorUsingForwardVector(const FVector& InDirection, TObjectPtr<class AActor>& InOutTargetActorPointer);

	virtual void OnPickableDetectBoxOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	//

	friend class AUSAPlayerController;
};
