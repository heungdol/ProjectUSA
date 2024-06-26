// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Engine/DamageEvents.h"

#include "Item/USAItemBase.h"

#include "USAGameInstance.generated.h"

#define IPCONFIG_NAME "IPConfig.txt"
#define DEMOCONFIG_NAME "DemoConfig.txt"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API UUSAGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
//public:
	//virtual void Init() override;
	
protected:
	virtual void OnStart() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA DamageType Info")
	TSubclassOf<class UDamageType> USADamageType_Explosion;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA DamageType Info")
	TSubclassOf<class UDamageType> USADamageType_Grab;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Item Info")
	TArray<TSubclassOf<class AUSAItemBase>> USAItemOrder;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Nickname Info")
	TArray<FString> USANicknames;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Nickname Info")
	FString LevelName_Title;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Nickname Info")
	FString LevelName_MainGame;

public:
	FORCEINLINE TSubclassOf<class UDamageType> GetUSADamageType_Explosion() {return USADamageType_Explosion;}

	FORCEINLINE TSubclassOf<class UDamageType> GetUSADamageType_Grab() { return USADamageType_Grab; }

	FORCEINLINE TArray<TSubclassOf<class AUSAItemBase>> GetUSAItemOrder() { return USAItemOrder; }

public:

	UFUNCTION(BlueprintCallable)
	void SetUSAInputModeUI(class APlayerController* InPlayerController);

	UFUNCTION(BlueprintCallable)
	void SetUSAInputModeGame(class APlayerController* InPlayerController);

	//

	UFUNCTION(BlueprintCallable)
	void DemoOneButtonStart ();

	//UFUNCTION(BlueprintCallable)
	void DemoJoinServer (const FString& InIpAddress);

	//UFUNCTION(BlueprintCallable)
	void DemoOpenLevelAsHost ();

	UFUNCTION(BlueprintCallable)
	bool GetIsDemo ();

	UFUNCTION(BlueprintCallable)
	bool GetIsDemoHost();


	//

	UFUNCTION(BlueprintCallable)
	void HostSession(class APlayerController* InPlayerController, FName InLevelName, bool InLAN);

	UFUNCTION(BlueprintCallable)
	void JoinSessionByLAN(class APlayerController* InPlayerController);

	UFUNCTION(BlueprintCallable)
	void JoinSessionByIPAddress(class APlayerController* InPlayerController, const FText& InIP);

	//

	UFUNCTION(BlueprintCallable)
	FString GetPlayerNickByIndex (int32 InIndex);

	UFUNCTION(BlueprintCallable)
	bool ReadTextFromFile(const FString& InFileName, FString& OutIPAddress);
	
	UFUNCTION(BlueprintCallable)
	bool GetIsValidIPAddress(const FString& InIPAddress);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void K2_HostSession(class APlayerController* InPlayerController, FName InLevelName, bool InLAN);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_JoinSessionByLAN(class APlayerController* InPlayerController);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_JoinSessionByIPAddress(class APlayerController* InPlayerController, const FText& InIP);

	//UFUNCTION(BlueprintImplementableEvent)
	//bool K2_EventTestReturnBoolean(int32 InTestInput);


};
