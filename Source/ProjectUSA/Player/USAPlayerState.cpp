// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/USAPlayerState.h"

#include "AbilitySystemComponent.h"

#include "GAS/AttributeSet/USAAttributeSet.h"


AUSAPlayerState::AUSAPlayerState()
{
    ASC = CreateDefaultSubobject <UAbilitySystemComponent>(TEXT("Ability System Component"));
    AttributeSet = CreateDefaultSubobject <UUSAAttributeSet>(TEXT("USA Attribute Set"));

    // 멀티 활성화
    //if (ASC != nullptr)
    //{
    //    ASC->SetIsReplicated(true);
    //}
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
