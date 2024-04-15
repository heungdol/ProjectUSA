// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"

#define USA_CHARACTER_ADJUST_IGNOREROTATETOMOVE FGameplayTag::RequestGameplayTag(FName("Character.Adjust.IgnoreRotateToMove"))
#define USA_CHARACTER_ADJUST_IGNOREMOVEINPUT FGameplayTag::RequestGameplayTag(FName("Character.Adjust.IgnoreMoveInput"))
#define USA_CHARACTER_ADJUST_VELOCITYZERO FGameplayTag::RequestGameplayTag(FName("Character.Adjust.VelocityZero"))
#define USA_CHARACTER_ADJUST_ROTATETOMOVEINPUT FGameplayTag::RequestGameplayTag(FName("Character.Adjust.RotateToMoveInput"))
#define USA_CHARACTER_ADJUST_CANNOTWALKOFFLEDGE FGameplayTag::RequestGameplayTag(FName("Character.Adjust.CanNotWalkOffLedge"))

#define USA_CHARACTER_STATE_CROUCH FGameplayTag::RequestGameplayTag(FName("Character.State.Crouch"))

#define USA_CHARACTER_ACTION_SLIDE FGameplayTag::RequestGameplayTag(FName("Character.Action.Slide"))