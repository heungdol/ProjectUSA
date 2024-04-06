// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"

#define USA_CHARACTER_STATE_FIXROTATION FGameplayTag::RequestGameplayTag(FName("Character.State.IgnoreRotateToMove"))
#define USA_CHARACTER_STATE_IGNOREMOVEINPUT FGameplayTag::RequestGameplayTag(FName("Character.State.IgnoreMoveInput"))
#define USA_CHARACTER_STATE_SLIDE FGameplayTag::RequestGameplayTag(FName("Character.State.Slide"))