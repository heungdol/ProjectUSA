// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/USACharacterBase.h"

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf <class AUSATargetingCameraActor> TargetingCameraActorClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr <class AUSATargetingCameraActor> TargetingCameraActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr <class APlayerController> PlayerController;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Camera Shake")
	TMap <TSubclassOf<class UDamageType>, TSubclassOf<class UCameraShakeBase>> HitSuccessCameraShakes;


public:
	AUSACharacterPlayer();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Move(const struct FInputActionValue& Value) override;
	
	virtual void OnRep_PlayerState() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void InitPlayerController();
	void InitTargetingCameraActor();

	//

	int8 bIsTargetingCamera : 1;
	virtual void DoTarget(const struct FInputActionValue& Value) override;
	
	void StartTargeting();
	void KeepTargeting();
	void ChangeTargeting();
	void FinishTargeting();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnStartTargeting", ScriptName = "OnStartTargeting"))
	void K2_OnStartTargeting();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnKeepTargeting", ScriptName = "OnStartTargeting"))
	void K2_OnKeepTargeting();
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnFinishTargeting", ScriptName = "OnFinishTargeting"))
	void K2_OnFinishTargeting();

	//

	virtual void DoDrop(const struct FInputActionValue& Value) override;

	//

	virtual void StartCameraShake_HitSuccess (TSubclassOf<class UDamageType> DamageType) override;


protected:
	virtual void Look(const struct FInputActionValue& Value) override;
	
	virtual void SetupGAS() override;
	virtual void OnRep_ASC() override;

	void UpdateCurrentTargetableActor() override;
	void UpdateCurrentTargetableActor_Instant() override;

	void SetCurrentTargetableActorUsingForwardVector(const FVector& InDirection);

	virtual void OnWeaponDetectBoxOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;


};
