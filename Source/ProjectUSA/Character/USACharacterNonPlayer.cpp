// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USACharacterNonPlayer.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

#include "ProjectUSA.h"


AUSACharacterNonPlayer::AUSACharacterNonPlayer()
{
	ASC = CreateDefaultSubobject <UAbilitySystemComponent>(TEXT("Ability System Component"));
}

void AUSACharacterNonPlayer::SetupGAS()
{
	if (ASC != nullptr)
	{
		ASC->InitAbilityActorInfo(this, this);
	}

	Super::SetupGAS();
}

float AUSACharacterNonPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 데미지 계산은 서버에서만 수행
	// 넉백 어빌리티는 서버와 클라 동시에 수행해야 함

	float ResultDamage = 0.0f;

	if (HasAuthority())
	//if (EventInstigator != nullptr
	//	&& DamageCauser != nullptr)
	{
		ResultDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
		USA_LOG(LogTemp, Log, TEXT("Get Damage: %f from %s %s"), Damage, *EventInstigator->GetName(), *DamageCauser->GetName());
	}

	return ResultDamage;
}
