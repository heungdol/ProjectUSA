// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/USAPlayerState.h"

#include "AbilitySystemComponent.h"

AUSAPlayerState::AUSAPlayerState()
{
    ASC = CreateDefaultSubobject <UAbilitySystemComponent>(TEXT("Ability System Component"));
}

UAbilitySystemComponent* AUSAPlayerState::GetAbilitySystemComponent() const
{
    return ASC;
}
