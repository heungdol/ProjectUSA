// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"

#define USA_CHARACTER_ACTION FGameplayTag::RequestGameplayTag(FName("Character.Action"))

#define USA_CHARACTER_ADJUST_IGNOREROTATETOMOVE FGameplayTag::RequestGameplayTag(FName("Character.Adjust.IgnoreRotateToMove"))
#define USA_CHARACTER_ADJUST_IGNOREMOVEINPUT FGameplayTag::RequestGameplayTag(FName("Character.Adjust.IgnoreMoveInput"))
#define USA_CHARACTER_ADJUST_VELOCITYZERO FGameplayTag::RequestGameplayTag(FName("Character.Adjust.VelocityZero"))
#define USA_CHARACTER_ADJUST_ROTATETOMOVEINPUT FGameplayTag::RequestGameplayTag(FName("Character.Adjust.RotateToMoveInput"))
#define USA_CHARACTER_ADJUST_CANNOTWALKOFFLEDGE FGameplayTag::RequestGameplayTag(FName("Character.Adjust.CanNotWalkOffLedge"))
#define USA_CHARACTER_ADJUST_OFFOVERLAPEVENT FGameplayTag::RequestGameplayTag(FName("Character.Adjust.OffOverlapEvent"))

#define USA_CHARACTER_STATE_CROUCH FGameplayTag::RequestGameplayTag(FName("Character.State.Crouch"))
#define USA_CHARACTER_STATE_WALK FGameplayTag::RequestGameplayTag(FName("Character.State.Walk"))
#define USA_CHARACTER_STATE_RUN FGameplayTag::RequestGameplayTag(FName("Character.State.Run"))
#define USA_CHARACTER_STATE_FALL FGameplayTag::RequestGameplayTag(FName("Character.State.Fall"))
#define USA_CHARACTER_STATE_DEAD FGameplayTag::RequestGameplayTag(FName("Character.State.Dead"))

#define USA_CHARACTER_ACTION FGameplayTag::RequestGameplayTag(FName("Character.Action"))
#define USA_CHARACTER_ACTION_SLIDE FGameplayTag::RequestGameplayTag(FName("Character.Action.Slide"))
#define USA_CHARACTER_ACTION_PARRY FGameplayTag::RequestGameplayTag(FName("Character.Action.Parry"))
#define USA_CHARACTER_ACTION_DAMAGE FGameplayTag::RequestGameplayTag(FName("Character.Action.Damage"))

#define USA_CHARACTER_HAND_FIRSTWEAPON FGameplayTag::RequestGameplayTag(FName("Character.Hand.FirstWeapon"))
#define USA_CHARACTER_HAND_SECONDWEAPON FGameplayTag::RequestGameplayTag(FName("Character.Hand.SecondWeapon"))