// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "USAHUD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API AUSAHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitCharacterHUD(class AUSACharacterPlayer* InPlayer);

	//

	UFUNCTION(BlueprintCallable)
	void PlayUserWidgetAnimation_Panel(bool bIsShowing, bool bIsRaw);

	//

	UFUNCTION(BlueprintCallable)
	void ShowHideCharacterHUD(bool bIsShowing);

	//

	UFUNCTION(BlueprintCallable)
	void ShowHideBossHPBar(bool bIsShowing);

	UFUNCTION(BlueprintCallable)
	void UpdateBossName(FName InName);

	UFUNCTION(BlueprintCallable)
	void UpdateBossHealthRatio(float InHealth);


	//

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerWeapon(EUSAWeaponType InType, class AUSAWeaponBase* InWeapon);

	UFUNCTION(BlueprintCallable)
	void UpdatePlayerHPBar(float InRatio);

	//

	UFUNCTION(BlueprintCallable)
	void ChangePlayerItem(TSubclassOf<class AUSAItemBase> InItemClass);

	UFUNCTION(BlueprintCallable)
	void ChangePlayerItemCount(int32 InItemCount);

	//

	UFUNCTION()
	void InitLocalPlayerController(class APlayerController* InPlayerController, class AUSAPlayerController* InUSAPlayerController);


protected:
	virtual void BeginPlay() override;

	//

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "USA Player Controller Info");
	//TObjectPtr <class UUserWidget> Screen_PlayerHUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Player Controller")
	TObjectPtr<class APlayerController> LocalPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Player Controller")
	TObjectPtr<class AUSAPlayerController> LocalUSAPlayerController;

	//

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "InitPlayerHUD", ScriptName = "InitPlayerHUD"))
	void K2_InitPlayerHUD(class AUSACharacterPlayer* InPlayer);

	//

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "UpdatePlayerWeapon", ScriptName = "UpdatePlayerWeapon"))
	void K2_UpdatePlayerWeapon(EUSAWeaponType InType, class AUSAWeaponBase* InWeapon);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "UpdatePlayerHealth", ScriptName = "UpdatePlayerHealth"))
	void K2_UpdatePlayerHPBar(float InRatio);


	//

	UFUNCTION(BlueprintImplementableEvent)
	void K2_ChangePlayerItem(TSubclassOf<class AUSAItemBase> InItemClass);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_ChangePlayerItemCount(int32 InItemCount);


	//

	UFUNCTION(BlueprintImplementableEvent)
	void K2_PlayUserWidgetAnimation_PanelShow();

	UFUNCTION(BlueprintImplementableEvent)
	void K2_PlayUserWidgetAnimation_PanelHide();

	UFUNCTION(BlueprintImplementableEvent)
	void K2_PlayUserWidgetAnimation_PanelBlack();

	UFUNCTION(BlueprintImplementableEvent)
	void K2_PlayUserWidgetAnimation_PanelWhite();

	//

	UFUNCTION(BlueprintImplementableEvent)
	void K2_ShowHideUserWidget_PlayerHUD(bool bIsShowing);

	//

	UFUNCTION(BlueprintImplementableEvent)
	void K2_ShowHideUserWidget_BossHPBar(bool bIsShowing);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_UpdateBossName(FName InName);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_UpdateBossHealthRatio(float InRatio);
};
