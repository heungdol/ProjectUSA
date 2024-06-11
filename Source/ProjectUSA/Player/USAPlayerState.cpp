// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/USAPlayerState.h"

#include "AbilitySystemComponent.h"

#include "GAS/AttributeSet/USAAttributeSet.h"

#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/HUD.h"

#include "Player/USAPlayerController.h"
#include "HUD/USAHUD.h"

#include "Character/USACharacterBase.h"

#include "GameInstance/USAGameInstance.h"

#include "Net/UnrealNetwork.h"


AUSAPlayerState::AUSAPlayerState()
{
    // 아래 컴포넌트들의 이름을 바꾸면 인식을 못하는 것 같다.
    ASC = CreateDefaultSubobject <UAbilitySystemComponent>(TEXT("Ability System Component"));
    AttributeSet = CreateDefaultSubobject <UUSAAttributeSet>(TEXT("USA Attribute Set"));

    // 멀티 활성화
    if (ASC != nullptr)
    {
        ASC->SetIsReplicated(true);
    }

    NetUpdateFrequency = 100.0f;
}

void AUSAPlayerState::SetPlayerIndex(int32 InIndex)
{
    PlayerIndex = InIndex;

    if (GetWorld()->GetAuthGameMode())
    {
        OnRep_PlayerIndex();
    }
}

void AUSAPlayerState::OnRep_PlayerIndex()
{
    //if (IsValid(ASC) == false)
    //{
    //    return;
    //}

    //if (IsValid(ASC->GetAvatarActor()) == false)
    //{
    //    return;
    //}

    //AUSACharacterBase* USACharacter = Cast <AUSACharacterBase>(ASC->GetAvatarActor());

    //if (IsValid(USACharacter) == false)
    //{
    //    return;
    //}

    //UUSAGameInstance* GameInstance = Cast <UUSAGameInstance>(GetGameInstance());

    //if (IsValid(GameInstance) == false)
    //{
    //    return;
    //}

    //USACharacter->ChangeCharacterName(GameInstance->GetPlayerNickByIndex (PlayerIndex));
}

UAbilitySystemComponent* AUSAPlayerState::GetAbilitySystemComponent() const
{
    return ASC;
}

float AUSAPlayerState::GetLookSensitivityRatio()
{
    if (bIsUsingGamepad == true) 
    {
        return LookSensitivityGamepadRatio;
    }
    
    return LookSensitivityMouseRatio;
}

void AUSAPlayerState::SetIsUsingGamepad(bool InUsing)
{
    bIsUsingGamepad = InUsing;
}

void AUSAPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AUSAPlayerState, PlayerIndex);
}