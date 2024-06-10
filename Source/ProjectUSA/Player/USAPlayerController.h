// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "Enum/USAEnums.h"
#include "Struct/USAStructs.h"

#include "USAPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTUSA_API AUSAPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Player Controller")
	TObjectPtr<class AUSAHUD> LocalUSAHUD;

	virtual void BeginPlay() override;

	virtual void BeginPlayingState() override;

//public:
//	UFUNCTION(BlueprintCallable)
//	void PlayUserWidgetAnimation_Panel(bool bIsShowing, bool bIsRaw);
//
//	UFUNCTION(Server, Reliable)
//	void ServerRPC_PlayUserWidgetAnimation_Panel(bool bIsShowing, bool bIsRaw);
//
//	UFUNCTION(NetMulticast, Reliable)
//	void MulticastRPC_PlayUserWidgetAnimation_Panel(bool bIsShowing, bool bIsRaw);

};
