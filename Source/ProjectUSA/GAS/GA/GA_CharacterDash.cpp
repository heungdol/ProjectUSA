// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GA/GA_CharacterDash.h"

#include "GAS/AT/AT_MoveToLocationByVelocity.h"

#include "Components/CapsuleComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AbilitySystemBlueprintLibrary.h"

#include "Character/USACharacterBase.h"

#include "ProjectUSA.h"

UGA_CharacterDash::UGA_CharacterDash()
{

}

void UGA_CharacterDash::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);
}

void UGA_CharacterDash::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
}

void UGA_CharacterDash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ACharacter* MyCharacter = nullptr;
	UCharacterMovementComponent* MyCharacterMovementComponent = nullptr;
	
	if (ActorInfo != nullptr)
	{
		MyCharacter = Cast <ACharacter>(ActorInfo->AvatarActor);
	}

	if (MyCharacter != nullptr)
	{
		MyCharacterMovementComponent = MyCharacter->GetCharacterMovement();
	}

	if (MyCharacter != nullptr
		&& MyCharacterMovementComponent != nullptr)
	{
		DashForwardDirection = MyCharacter->GetPendingMovementInputVector();

		if (HasAuthority(&ActivationInfo) == false
			&& GetAvatarActorFromActorInfo()->GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
		{
			//USA_LOG_GAMEPLAYABILITY(LogTemp, Log, TEXT("Only Client"));

			ServerRPC_SetDashForwardDirection(MyCharacter->GetPendingMovementInputVector());

			DoDash();
		}
		else if (HasAuthority(&ActivationInfo) == true
			&& GetAvatarActorFromActorInfo()->GetLocalRole() == ENetRole::ROLE_Authority
			&& GetAvatarActorFromActorInfo()->GetRemoteRole() == ENetRole::ROLE_SimulatedProxy)
		{
			//USA_LOG_GAMEPLAYABILITY(LogTemp, Log, TEXT("Only Server"));

			DoDash();
		}
	}
	else
	{
		SimpleCancelAbility();
	}
}

void UGA_CharacterDash::DoDash()
{
	ACharacter* MyCharacter = nullptr;
	UCharacterMovementComponent* MyCharacterMovementComponent = nullptr;

	if (CurrentActorInfo != nullptr)
	{
		MyCharacter = Cast <ACharacter>(CurrentActorInfo->AvatarActor);
	}

	if (MyCharacter != nullptr)
	{
		MyCharacterMovementComponent = MyCharacter->GetCharacterMovement();
	}

	if (MyCharacter != nullptr
		&& MyCharacterMovementComponent != nullptr)
	{
		FVector ForwardDirection = MyCharacter->GetActorForwardVector();
		FVector RightDirection = MyCharacter->GetActorRightVector();

		if (DashForwardDirection.Length() > SMALL_NUMBER/* != FVector::ZeroVector*/)
		{
			FVector InputVector = DashForwardDirection;

			ForwardDirection = InputVector;
			ForwardDirection.Normalize();

			RightDirection = FVector::CrossProduct(FVector::UpVector, ForwardDirection);
			RightDirection.Normalize();

			MyCharacter->SetActorRotation(ForwardDirection.Rotation());
		}

		FVector EndLocation(0, 0, 0);
		EndLocation = MyCharacter->GetActorLocation()
			+ (ForwardDirection * DashOffsetLocation.X)
			+ (RightDirection * DashOffsetLocation.Y)
			+ (FVector::UpVector * DashOffsetLocation.Z);

		UAT_MoveToLocationByVelocity* AbilityTask = UAT_MoveToLocationByVelocity::GetNewAbilityTask
		(this, TEXT("Dash"), EndLocation, DashDuration, DashCurveFloat, nullptr);

		AbilityTask->OnTargetLocationReached.AddDynamic(this, &UGA_CharacterDash::SimpleEndAbility);
		AbilityTask->ReadyForActivation();

		UAT_PlayAnimMontages* AbilityTaskMontage = UAT_PlayAnimMontages::GetNewAbilityTask(this, DashAnimMontageData);
		OnEndAbility.AddUObject(AbilityTaskMontage, &UAT_PlayAnimMontages::SimpleEndAbilityTask);
		OnCancelAbility.AddUObject(AbilityTaskMontage, &UAT_PlayAnimMontages::SimpleEndAbilityTask);

		AbilityTaskMontage->ReadyForActivation();
	}
	else
	{
		SimpleCancelAbility();
	}
}

void UGA_CharacterDash::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGA_CharacterDash::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UGA_CharacterDash::ServerRPC_SetDashForwardDirection_Validate(const FVector& InDirection)
{
	return true;
}


void UGA_CharacterDash::ServerRPC_SetDashForwardDirection_Implementation(const FVector& InDirection)
{
	USA_LOG_GAMEPLAYABILITY(LogTemp, Log, TEXT("called in server"));

	DashForwardDirection = InDirection;

	DoDash();
}
