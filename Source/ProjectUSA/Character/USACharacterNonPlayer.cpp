// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USACharacterNonPlayer.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

#include "Debug/DebugDrawComponent.h"

#include "Engine/DamageEvents.h"
#include "GameFramework/DamageType.h"

#include "GAS/AttributeSet/USAAttributeSet.h"

#include "ProjectUSA.h"


AUSACharacterNonPlayer::AUSACharacterNonPlayer()
{
	ASC = CreateDefaultSubobject <UAbilitySystemComponent>(TEXT("Ability System Component"));
	AttributeSet = CreateDefaultSubobject <UUSAAttributeSet>(TEXT("USA Attribute Set"));
}

void AUSACharacterNonPlayer::SetupGAS()
{
	if (ASC != nullptr)
	{
		ASC->InitAbilityActorInfo(this, this);
	}

	Super::SetupGAS();

	//UUSAAttributeSet* USAAttributeSet = nullptr;

	//if (ASC != nullptr)
	//{
	//	ASC->InitAbilityActorInfo(this, this);
	//	USAAttributeSet = const_cast<UUSAAttributeSet*>(ASC->GetSet<UUSAAttributeSet>());
	//}

	//if (USAAttributeSet != nullptr)
	//{
	//	USAAttributeSet->OnOutOfHealth.AddUObject(this, &AUSACharacterBase::OnUSADeath);
	//}

	//Super::SetupGAS();
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

		FVector AttackDirection;
		FHitResult HitResult;
		
		DamageEvent.GetBestHitInfo(nullptr, nullptr, HitResult, AttackDirection);

		USA_LOG(LogTemp, Log, TEXT("Get Damage: %f from %s %s"), Damage, *EventInstigator->GetName(), *DamageCauser->GetName());

		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + AttackDirection * 300.0f, FColor::Emerald, false, 0.5f, 0U, 2.0f);
	}

	return ResultDamage;
}
