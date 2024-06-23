// Fill out your copyright notice in the Description page of Project Settings.


#include "Struct/USAStructs.h"

#include "AbilitySystemComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Engine/DamageEvents.h"
#include "GameFramework/DamageType.h"


#include "Components/CapsuleComponent.h"

#include "Component/USAJellyEffectComponent.h"

#include "GAS/AttributeSet/USAAttributeSet.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"

#include "Interface/USADamageableInterface.h"


// ====================================================================================


void FUSACharacterCapsuleInfo::RenewCharacterCapsule(ACharacter* InCharacter, USpringArmComponent* InSpringArmComponent)
{
	RenewCharacterCapsuleLocation(InCharacter, InSpringArmComponent);
	RenewCharacterCapsuleSize(InCharacter);
}

void FUSACharacterCapsuleInfo::RenewCharacterCapsuleSize(ACharacter* InCharacter)
{
	if (InCharacter == nullptr)
	{
		return;
	}

	if (InCharacter->GetCapsuleComponent() == nullptr)
	{
		return;
	}

	InCharacter->GetCapsuleComponent()->SetCapsuleSize(CapsuleRadius, CapsuleHaflHeight);
	//InCharacter->GetCharacterMovement()->AdjustProxyCapsuleSize();
}

void FUSACharacterCapsuleInfo::RenewCharacterCapsuleLocation(ACharacter* InCharacter, USpringArmComponent* InSpringArmComponent)
{
	if (InCharacter == nullptr)
	{
		return;
	}

	// 카메라 예외 처리
	FVector CameraSpringArmLocation = FVector::ZeroVector;
	if (IsValid(InSpringArmComponent))
	{
		CameraSpringArmLocation = InSpringArmComponent->GetComponentLocation();
	}

	// 위치 갱신

	UCharacterMovementComponent* CharacterMovementComponent = InCharacter->GetCharacterMovement();

	if (CharacterMovementComponent == nullptr)
	{
		return;
	}

	if (InCharacter->GetCapsuleComponent() == nullptr)
	{
		return;
	}

	if (InCharacter->GetMesh() == nullptr)
	{
		return;
	}

	// 땅에 파고 들지 않기 위한 보정
	FVector GroundHitLocation = FVector::ZeroVector;
	if (CharacterMovementComponent->CurrentFloor.bBlockingHit)
	{
		GroundHitLocation = CharacterMovementComponent->CurrentFloor.HitResult.ImpactPoint;
	}

	FVector NewLocation = InCharacter->GetActorLocation();
	//FVector OffsetLocation = FVector::ZeroVector;

	switch (CapsulePivot)
	{
	case EUSACharacterCapsulePivot::Top:
		NewLocation.Z += -(CapsuleHaflHeight - InCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		//OffsetLocation.Z = -(CapsuleHaflHeight - InCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		break;

	case EUSACharacterCapsulePivot::Center:
		break;

	case EUSACharacterCapsulePivot::Bottom:
		NewLocation.Z += (CapsuleHaflHeight - InCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		//OffsetLocation.Z = (CapsuleHaflHeight - InCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		break;
	}

	if (CharacterMovementComponent->CurrentFloor.bBlockingHit &&
		GroundHitLocation.Z > NewLocation.Z - CapsuleHaflHeight)
	{
		NewLocation.Z = GroundHitLocation.Z + CapsuleHaflHeight;
	}

	InCharacter->SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);

	// 메쉬 위치 갱신

	FVector NewUpdatedComponentsLocation = FVector::ZeroVector;

	switch (CapsulePivot)
	{
	case EUSACharacterCapsulePivot::Top:
		NewUpdatedComponentsLocation = FVector::UpVector * -(CapsuleOriginalHalfHeight * 2.0f - CapsuleHaflHeight);
		break;

	case EUSACharacterCapsulePivot::Center:
		NewUpdatedComponentsLocation = FVector::UpVector * -(CapsuleOriginalHalfHeight);
		break;

	case EUSACharacterCapsulePivot::Bottom:
		NewUpdatedComponentsLocation = FVector::UpVector * -(CapsuleHaflHeight);
		break;
	};

	// Authority or Automonous Mesh
	InCharacter->GetMesh()->SetRelativeLocation(NewUpdatedComponentsLocation);

	// Simulated Mesh
	InCharacter->CacheInitialMeshOffset(NewUpdatedComponentsLocation, FRotator(0.0f, -90.0f, 0.0f));

	// 카메라 예외 처리
	if (IsValid(InSpringArmComponent))
	{
		InSpringArmComponent->SetWorldLocation(CameraSpringArmLocation);
	}
}

void FUSACharacterCapsuleInfo::RenewJellyEffectMeshLocation(UUSAJellyEffectComponent* InJellyEffect)
{
	if (InJellyEffect == nullptr)
	{
		return;
	}

	FVector NewUpdatedComponentsLocation = FVector::ZeroVector;

	switch (CapsulePivot)
	{
	case EUSACharacterCapsulePivot::Top:
		NewUpdatedComponentsLocation = FVector::UpVector * -(CapsuleOriginalHalfHeight * 2.0f - CapsuleHaflHeight);
		break;

	case EUSACharacterCapsulePivot::Center:
		NewUpdatedComponentsLocation = FVector::UpVector * -(CapsuleOriginalHalfHeight);
		break;

	case EUSACharacterCapsulePivot::Bottom:
		NewUpdatedComponentsLocation = FVector::UpVector * -(CapsuleHaflHeight);
		break;
	};

	InJellyEffect->SetMeshStartLocation(NewUpdatedComponentsLocation);
}


// ====================================================================================


void FUSACharacterAttributeSetInfo::RenewUSACharacterAttributeSetData(UAbilitySystemComponent* InASC)
{
	if (InASC == nullptr)
	{
		return;
	}

	if (InASC->GetSet <UUSAAttributeSet>() != nullptr)
	{
		InASC->SetNumericAttributeBase(UUSAAttributeSet::GetMaxHealthAttribute(), StartMaxHealth);
		InASC->SetNumericAttributeBase(UUSAAttributeSet::GetCurrentHealthAttribute(), StartCurrentHealth);
		InASC->SetNumericAttributeBase(UUSAAttributeSet::GetBaseArmorAttribute(), StartBaseArmor);
		InASC->SetNumericAttributeBase(UUSAAttributeSet::GetCurrentArmorAttribute(), StartBaseArmor);
	}
}

// ====================================================================================

FAttackTraceSceneInfo::FAttackTraceSceneInfo()
{

}

FAttackTraceSceneInfo::FAttackTraceSceneInfo
(UWorld* InWorld, const FAttackTraceInfo& InDefault, AActor* InSource, 
	const FVector& InLocation,
	const FVector& InFoward, const FVector& InRight, const FVector& InUp)
{
	DefaultAttackTraceInfo.AttackDamage = InDefault.AttackDamage;
	DefaultAttackTraceInfo.AttackDamageType = InDefault.AttackDamageType;
	DefaultAttackTraceInfo.AttackDuration = InDefault.AttackDuration;
	//DefaultAttackTraceInfo.AttackTraceDuration = InDefault.AttackTraceDuration;

	DefaultAttackTraceInfo.AttackTime = InDefault.AttackTime;
	DefaultAttackTraceInfo.AttackTraceRadius = InDefault.AttackTraceRadius;
	DefaultAttackTraceInfo.AttackHitNiagaraSystemObject = InDefault.AttackHitNiagaraSystemObject;
	
	DefaultAttackTraceInfo.bIsAttackHitNiagaraOffset = InDefault.bIsAttackHitNiagaraOffset;

	//DefaultAttackTraceInfo.AttackHitNiagaraSystemObjectRandomRatioX = InDefault.AttackHitNiagaraSystemObjectRandomRatioX;
	//DefaultAttackTraceInfo.AttackHitNiagaraSystemObjectRandomRatioY = InDefault.AttackHitNiagaraSystemObjectRandomRatioY;
	//DefaultAttackTraceInfo.AttackHitNiagaraSystemObjectRandomRatioZ = InDefault.AttackHitNiagaraSystemObjectRandomRatioZ;
	
	DefaultAttackTraceInfo.bIsDirectToTarget = InDefault.bIsDirectToTarget;
	DefaultAttackTraceInfo.bIsPinnedLocation = InDefault.bIsPinnedLocation;
	DefaultAttackTraceInfo.bIsPinnedRotation = InDefault.bIsPinnedRotation;
	DefaultAttackTraceInfo.bIsUsingSigleTrace = InDefault.bIsUsingSigleTrace;

	DefaultAttackTraceInfo.OffsetTraceLocation = InDefault.OffsetTraceLocation;
	DefaultAttackTraceInfo.OffsetTraceEndLocation = InDefault.OffsetTraceEndLocation;

	DefaultAttackTraceInfo.AttackEndGameplayTag_Added = InDefault.AttackEndGameplayTag_Added;
	DefaultAttackTraceInfo.AttackEndGameplayTag_Removed = InDefault.AttackEndGameplayTag_Removed;

	//

	AttackLocation = InLocation;

	//

	AttackForwardDirection = InFoward;
	AttackRightDirection = InRight;
	AttackUpDirection= InUp;

	//

	StartAttackTime = InWorld->GetTimeSeconds();

	AttackSourceActor = InSource;

	CheckedActorList.Init(InSource, 1);
}

void FAttackTraceSceneInfo::DoAttackTrace(UWorld* InWorld, float DeltaTime)
{
	ACharacter* MyCharacter = Cast <ACharacter>(AttackSourceActor);

	//if (GetIsAttackTraceEnded(InWorld) == true)
	//{
	//	return;
	//}

	if (IsValid(MyCharacter) == false)
	{
		return;
	}

	if (IsValid(InWorld) == false)
	{
		return;
	}

	if (IsValid(AttackSourceActor) == false)
	{
		return;
	}

	//

	FVector AttackStartTraceLocation = AttackSourceActor->GetActorLocation();
	if (DefaultAttackTraceInfo.bIsPinnedLocation)
	{
		AttackStartTraceLocation = AttackLocation;
	}

	AttackStartTraceLocation += AttackSourceActor->GetActorForwardVector() * DefaultAttackTraceInfo.OffsetTraceLocation.X;
	AttackStartTraceLocation += AttackSourceActor->GetActorRightVector() * DefaultAttackTraceInfo.OffsetTraceLocation.Y;
	AttackStartTraceLocation += AttackSourceActor->GetActorUpVector() * DefaultAttackTraceInfo.OffsetTraceLocation.Z;

	//

	FVector AttackDirection = MyCharacter->GetActorForwardVector();
	FVector AttackEndTraceLocation = AttackStartTraceLocation;

	if (DefaultAttackTraceInfo.bIsPinnedRotation)
	{
		AttackDirection = AttackForwardDirection;
		AttackDirection.Z = 0;
		AttackDirection.Normalize();

		AttackEndTraceLocation += AttackForwardDirection * DefaultAttackTraceInfo.OffsetTraceEndLocation.X;
		AttackEndTraceLocation += AttackRightDirection * DefaultAttackTraceInfo.OffsetTraceEndLocation.Y;
		AttackEndTraceLocation += AttackUpDirection * DefaultAttackTraceInfo.OffsetTraceEndLocation.Z;
	}
	else
	{
		AttackDirection = MyCharacter->GetActorForwardVector();

		AttackEndTraceLocation += MyCharacter->GetActorForwardVector() * DefaultAttackTraceInfo.OffsetTraceEndLocation.X;
		AttackEndTraceLocation += MyCharacter->GetActorRightVector() * DefaultAttackTraceInfo.OffsetTraceEndLocation.Y;
		AttackEndTraceLocation += MyCharacter->GetActorUpVector() * DefaultAttackTraceInfo.OffsetTraceEndLocation.Z;
	}

	//

	float AttackDamage = DefaultAttackTraceInfo.AttackDamage;
	float AttackTraceRadius = DefaultAttackTraceInfo.AttackTraceRadius;

	if (DefaultAttackTraceInfo.bIsUsingSigleTrace == true)
	{
		FHitResult HitResult;
		UKismetSystemLibrary::SphereTraceSingle
		(InWorld,
			AttackStartTraceLocation,
			AttackEndTraceLocation,
			AttackTraceRadius,
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Pawn),
			false,
			CheckedActorList,
			EDrawDebugTrace::None,
			HitResult,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			0.1f);

		if (HitResult.bBlockingHit == true)
		{
			if (IsValid(HitResult.GetActor()) == true)
			{
				CheckedActorList.Add(HitResult.GetActor());
			}

			IUSADamageableInterface* USADamageableInterface = Cast<IUSADamageableInterface>(HitResult.GetActor());

			if (USADamageableInterface)
			{
				TSubclassOf<UDamageType> AttackDamageType = DefaultAttackTraceInfo.AttackDamageType;
				FPointDamageEvent AttackDamageEvent = FPointDamageEvent(AttackDamage, HitResult, AttackDirection, AttackDamageType);

				USADamageableInterface->ApplyDamageHitNiagaraEffect(MyCharacter->GetController(), MyCharacter, 
					DefaultAttackTraceInfo.AttackHitNiagaraSystemObject,
					DefaultAttackTraceInfo.bIsAttackHitNiagaraOffset);

				USADamageableInterface->TakeDamage(AttackDamage, AttackDamageEvent, MyCharacter->GetController(), MyCharacter);
			}

		}
	}
	else
	{
		TArray<FHitResult> HitResults;
		UKismetSystemLibrary::SphereTraceMulti
		(InWorld,
			AttackStartTraceLocation,
			AttackEndTraceLocation,
			AttackTraceRadius,
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Pawn),
			false,
			CheckedActorList,
			EDrawDebugTrace::None,
			HitResults,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			0.1f);

		for (FHitResult HitResult : HitResults)
		{
			if (IsValid(HitResult.GetActor()) == true)
			{
				CheckedActorList.Add(HitResult.GetActor());
			}

			IUSADamageableInterface* USADamageableInterface = Cast<IUSADamageableInterface>(HitResult.GetActor());

			if (USADamageableInterface)
			{
				TSubclassOf<UDamageType> AttackDamageType = DefaultAttackTraceInfo.AttackDamageType;
				FPointDamageEvent AttackDamageEvent = FPointDamageEvent(AttackDamage, HitResult, AttackDirection, AttackDamageType);

				USADamageableInterface->ApplyDamageHitNiagaraEffect(MyCharacter->GetController(), MyCharacter, 
					DefaultAttackTraceInfo.AttackHitNiagaraSystemObject,
					DefaultAttackTraceInfo.bIsAttackHitNiagaraOffset);

				USADamageableInterface->TakeDamage(AttackDamage, AttackDamageEvent, MyCharacter->GetController(), MyCharacter);
			}

		}
	}

}

//bool FAttackTraceSceneInfo::GetIsAttackTraceEnded(UWorld* InWorld)
//{
//	bool Result = false;
//
//	if (IsValid(InWorld) == false)
//	{
//		return false;
//	}
//
//	if (IsValid(AttackSourceActor) == false)
//	{
//		return false;
//	}
//
//	Result = ((StartAttackTime + DefaultAttackTraceInfo.AttackTraceDuration) < InWorld->GetTimeSeconds());
//
//	return Result;
//}

bool FAttackTraceSceneInfo::GetIsAttackEnded(UWorld* InWorld)
{
	bool Result = false;

	if (IsValid(InWorld) == false)
	{
		return false;
	}

	if (IsValid(AttackSourceActor) == false)
	{
		return false;
	}

	Result = ((StartAttackTime + DefaultAttackTraceInfo.AttackDuration) < InWorld->GetTimeSeconds());

	return Result;
}
