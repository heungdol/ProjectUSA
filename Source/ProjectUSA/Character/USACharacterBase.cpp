// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USACharacterBase.h"

#include "Components/CapsuleComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Component/USACharacterMovementComponent.h"

#include "Camera/USACameraComponent.h"
#include "Camera/USASpringArmComponent.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

#include "GameFramework/PlayerState.h"

#include "Component/USAJellyEffectComponent.h"

#include "Tag/USAGameplayTags.h"


#include "Components/SkeletalMeshComponent.h"


#include "ProjectUSA.h"

// ====================================================================================

void FUSACharacterMovementWalkInfo::RenewCharacterMovementInfo(UCharacterMovementComponent* InMovementComponet)
{
	if (InMovementComponet == nullptr)
	{
		return;
	}

	InMovementComponet->MaxWalkSpeed = FUSACharacterMovementWalkInfo::MaxWalkSpeed;
	InMovementComponet->RotationRate = FUSACharacterMovementWalkInfo::RotationRate;
	InMovementComponet->MaxAcceleration = FUSACharacterMovementWalkInfo::MaxAcceleration;
	InMovementComponet->GroundFriction = FUSACharacterMovementWalkInfo::GroundFriction;
	InMovementComponet->BrakingDecelerationWalking = FUSACharacterMovementWalkInfo::BrakingDecelerationWalking;
}

//void FUSACharacterCapsuleInfo::RenewCharacterCapsule(ACharacter* InCharacter)
//{
//	if (InCharacter == nullptr)
//	{
//		return;
//	}
//
//	//bool bIsGrounded = false;
//	//FVector RenewLocation = FVector::ZeroVector;
//
//	//if (InCharacter->GetCharacterMovement() != nullptr
//	//	&& InCharacter->GetCharacterMovement()->IsFalling() == false)
//	//{
//	//	//bIsGrounded = true;
//
//	//	//RenewLocation = InCharacter->GetCharacterMovement()->CurrentFloor.HitResult.Location;
//	//	//RenewLocation += FVector::UpVector * CapsuleHeight * 0.5f;
//
//	//	FVector NewLocation = InCharacter->GetActorLocation();
//	//	NewLocation.Z += (CapsuleHeight * 0.5f - InCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
//
//	//	InCharacter->SetActorLocation(NewLocation);
//	//}
//
//	if (InCharacter->GetCharacterMovement() != nullptr
//		&& InCharacter->GetCharacterMovement()->IsFalling() == false
//		&& InCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() > CapsuleHaflHeight)
//	{
//		FVector NewLocation = InCharacter->GetActorLocation();
//		NewLocation.Z += (CapsuleHaflHeight + CapsuleRadius - InCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()) ;
//
//		InCharacter->SetActorLocation(NewLocation);
//	}
//
//	InCharacter->GetCapsuleComponent()->SetCapsuleHalfHeight(CapsuleHaflHeight);
//	InCharacter->GetCapsuleComponent()->SetCapsuleRadius(CapsuleRadius);
//
//	InCharacter->GetCapsuleComponent()->UpdateBodySetup();
//}


//void FUSACharacterCapsuleInfo::RenewCharacterCapsuleIncludeLocation(ACharacter* InCharacter)
//{
//	if (InCharacter == nullptr)
//	{
//		return;
//	}
//
//	float PrevCharacterCapsuleHalfHeight = InCharacter->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
//	FVector PrevCharacterLocation = InCharacter->GetActorLocation();
//
//	
//	float RenewCharacterCapsuleHalfHeight = CapsuleHeight * 0.5f;
//	FVector RenewCharacterLocaiton = PrevCharacterLocation + FVector::UpVector
//		* (RenewCharacterCapsuleHalfHeight - PrevCharacterCapsuleHalfHeight);
//
//	//UE_LOG(LogTemp, Log, TEXT("Prev %f, New %f"), PrevCharacterCapsuleHalfHeight, RenewCharacterCapsuleHalfHeight);
//
//
//	InCharacter->GetCapsuleComponent()->SetCapsuleHalfHeight(CapsuleHeight * 0.5f);
//	InCharacter->GetCapsuleComponent()->SetCapsuleRadius(CapsuleRadius);
//
//	InCharacter->GetMesh()->SetRelativeLocation(FVector(0, 0, CapsuleHeight * -0.5f));
//	
//	InCharacter->SetActorLocation(RenewCharacterLocaiton);
//}

// ====================================================================================
// ====================================================================================


// Sets default values
AUSACharacterBase::AUSACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 1.0f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 4000.f;
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

	CameraSpringArmComponent = CreateDefaultSubobject <UUSASpringArmComponent>(TEXT("Camera Spring Arm Component"));
	CameraSpringArmComponent->SetupAttachment(RootComponent);
	CameraSpringArmComponent->TargetArmLength = 500.0f;
	CameraSpringArmComponent->bUsePawnControlRotation = true;
	CameraSpringArmComponent->SetRelativeLocation(FVector(0, 0, 20.0f));

	CameraComponent = CreateDefaultSubobject <UUSACameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(CameraSpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

	JellyEffectComponent = CreateDefaultSubobject <UUSAJellyEffectComponent>(TEXT("Jelly Effect Component"));
	JellyEffectComponent->SetMeshComponent(GetMesh());

	//CharacterCapsuleWalkInfo.RenewCharacterCapsule(this);
	CharacterMovementWalkInfo.RenewCharacterMovementInfo(GetCharacterMovement());

	//GetCharacterMovement()->MovementState.bCanCrouch = true;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 0.0f;
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;

	ASC = nullptr;
}

void AUSACharacterBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	//CharacterCapsuleWalkInfo.RenewCharacterCapsule(this);
	CharacterMovementWalkInfo.RenewCharacterMovementInfo(GetCharacterMovement());
}

// Called when the game starts or when spawned
void AUSACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	//CharacterCapsuleWalkInfo.RenewCharacterCapsule(this);
	CharacterMovementWalkInfo.RenewCharacterMovementInfo(GetCharacterMovement());

	// 게임 어빌리티 부여
	// 캐릭터 파괴를 고려하여 BeginPlay 단에 옮겨 놓았다
	if (HasAuthority() == true)
	{
		if (ASC != nullptr)
		{
			for (const auto& GameplayTriggerAbility : GameplayTriggerAbilities)
			{
				FGameplayAbilitySpec GameplayAbilitySpec(GameplayTriggerAbility);
				ASC->GiveAbility(GameplayAbilitySpec);
			}

			for (const auto& GameplayActionAbility : GameplayActiveAbilities)
			{
				FGameplayAbilitySpec GameplayAbilityActionSpec(GameplayActionAbility.GameplayAbility);

				if (GameplayActionAbility.InputID >= 0)
				{
					GameplayAbilityActionSpec.InputID = GameplayActionAbility.InputID;
				}

				ASC->GiveAbility(GameplayAbilityActionSpec);
			}

			// 게임 시작 어빌리티
			for (const auto& GameplayStartAbility : GameplayStartAbilities)
			{
				FGameplayAbilitySpec GameplayAbilitySpec(GameplayStartAbility);
				ASC->GiveAbility(GameplayStartAbility);
			}
		
			for (const auto& GameplayStartAbility : GameplayStartAbilities)
			{
				FGameplayAbilitySpec* GameplayAbilitySpec = ASC->FindAbilitySpecFromClass(GameplayStartAbility);

				if (GameplayAbilitySpec == nullptr)
				{
					return;
				}

				if (GameplayAbilitySpec->IsActive())
				{
					ASC->AbilitySpecInputPressed(*GameplayAbilitySpec);
				}
				else
				{
					ASC->TryActivateAbility(GameplayAbilitySpec->Handle);
				}
			}

			//

			for (const auto& GameplayStartActionAbility : GameplayStartActionAbilites)
			{
				FGameplayAbilitySpec GameplayAbilitySpec(GameplayStartActionAbility);
				ASC->GiveAbility(GameplayStartActionAbility);
			}

			for (const auto& GameplayStartActionAbility : GameplayStartActionAbilites)
			{
				FGameplayAbilitySpec* GameplayAbilitySpec = ASC->FindAbilitySpecFromClass(GameplayStartActionAbility);

				if (GameplayAbilitySpec == nullptr)
				{
					return;
				}

				if (GameplayAbilitySpec->IsActive())
				{
					ASC->AbilitySpecInputPressed(*GameplayAbilitySpec);
				}
				else
				{
					ASC->TryActivateAbility(GameplayAbilitySpec->Handle);
				}
			}
		}
	}
}

// Called every frame
void AUSACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Log, TEXT("Z: %f"), GetActorLocation().Z);

	AdjustVelocityWithVelocityZero();
}

// Called to bind functionality to input
void AUSACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUSACharacterBase::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUSACharacterBase::Look);

		for (const auto& GameplayActiveAbility : GameplayActiveAbilities)
		{
			if (GameplayActiveAbility.InputID < 0)
			{
				continue;
			}

			//USA_LOG(LogTemp, Log, TEXT("Binding Functions related with GAS"));

			EnhancedInputComponent->BindAction(GameplayActiveAbility.InputAction, ETriggerEvent::Triggered,
				this, &AUSACharacterBase::InputPressGameplayAbilityByInputID, GameplayActiveAbility.InputID);
			EnhancedInputComponent->BindAction(GameplayActiveAbility.InputAction, ETriggerEvent::Completed,
				this, &AUSACharacterBase::InputReleaseGameplayAbilityByInputID, GameplayActiveAbility.InputID);
		}
	}
}

void AUSACharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 일반 클라에서는 수행되지 않음
	SetupGAS();

	// 시작할 때 자동으로 콘솔 입력
	APlayerController* PlayerController = Cast <APlayerController>(NewController);
	if (PlayerController != nullptr)
	{
		PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
	}
}

void AUSACharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 일반 클라에서 수행
	SetupGAS();
}

void AUSACharacterBase::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AUSACharacterBase::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AUSACharacterBase::InputPressGameplayAbilityByInputID(int32 InputID)
{
	if (ASC == nullptr)
	{
		return;
	}

	FGameplayAbilitySpec* GameplayAbilitySpec = ASC->FindAbilitySpecFromInputID(InputID);

	if (GameplayAbilitySpec == nullptr)
	{
		return;
	}

	if (GameplayAbilitySpec->IsActive())
	{
		ASC->AbilitySpecInputPressed(*GameplayAbilitySpec);
	}
	else
	{
		ASC->TryActivateAbility(GameplayAbilitySpec->Handle);
	}
}

void AUSACharacterBase::InputReleaseGameplayAbilityByInputID(int32 InputID)
{
	if (ASC == nullptr)
	{
		return;
	}

	FGameplayAbilitySpec* GameplayAbilitySpec = ASC->FindAbilitySpecFromInputID(InputID);
	if (GameplayAbilitySpec == nullptr)
	{
		return;
	}

	if (GameplayAbilitySpec->IsActive())
	{
		ASC->AbilitySpecInputReleased(*GameplayAbilitySpec);
	}
}

void AUSACharacterBase::TryGameplayAbilityByGameplayTag(FName GameplayTag)
{
	if (ASC == nullptr)
	{
		return;
	}

	if (GameplayTag == TEXT(""))
	{
		return;
	}

	FGameplayTagContainer TagContainer(FGameplayTag::RequestGameplayTag(GameplayTag));
	ASC->TryActivateAbilitiesByTag(TagContainer);
}

void AUSACharacterBase::GameplayTagIgnoreRotateToMoveCallback(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (GetCharacterMovement() == nullptr)
	{
		return;
	}

	if (NewCount > 0)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}

}

void AUSACharacterBase::GameplayTagIgnoreMoveInputCallback(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (GetController () == nullptr)
	{
		return;
	}

	if (NewCount > 0)
	{
		GetController()->SetIgnoreMoveInput (true);
	}
	else
	{
		GetController()->ResetIgnoreMoveInput ();
	}
}

void AUSACharacterBase::GameplayTagVelocityZeroCallback(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (GetCharacterMovement() == nullptr)
	{
		return;
	}

	if (NewCount > 0)
	{
		bIsVelocityZero = true;
		GetCharacterMovement()->bApplyGravityWhileJumping = false;
	}
	else
	{
		bIsVelocityZero = false;
		GetCharacterMovement()->bApplyGravityWhileJumping = true;
	}
}

void AUSACharacterBase::GameplayTagCanNotWalkOffLedgeCallback(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (GetCharacterMovement() == nullptr)
	{
		return;
	}

	if (NewCount > 0)
	{
		GetCharacterMovement()->bCanWalkOffLedges = false;
		GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = false;
	}
	else
	{
		GetCharacterMovement()->bCanWalkOffLedges = true;
		GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;
	}
}

void AUSACharacterBase::GameplayTagSlideCallback(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (GetCharacterMovement() == nullptr)
	{
		return;
	}

	if (NewCount > 0)
	{
		//CharacterCapsuleSlideInfo.RenewCharacterCapsule(this);
		CharacterMovementSlideInfo.RenewCharacterMovementInfo(GetCharacterMovement());
	}
	else
	{
		//CharacterCapsuleWalkInfo.RenewCharacterCapsule(this);
		CharacterMovementWalkInfo.RenewCharacterMovementInfo(GetCharacterMovement());
	}
}

void AUSACharacterBase::GameplayTagCrouchCallback(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

// TODO: 추후 중력 때문에 미약하게 낙하하는 이슈 수정
void AUSACharacterBase::AdjustVelocityWithVelocityZero()
{
	if (bIsVelocityZero)
	{
		FVector NewVelocity = GetCharacterMovement()->Velocity;
		NewVelocity.X = 0;
		NewVelocity.Y = 0;

		if (NewVelocity.Z > 0)
		{
			NewVelocity.Z = 0;
		}
		else
		{
			NewVelocity.Z = -NewVelocity.Z;
		}

		GetCharacterMovement()->Velocity = NewVelocity;
		GetCharacterMovement()->UpdateComponentVelocity();
	}
}

UAbilitySystemComponent* AUSACharacterBase::GetAbilitySystemComponent() const
{
	return ASC;
}

void AUSACharacterBase::SetupGAS()
{
	if (ASC == nullptr)
	{
		return;
	}

	ASC->RegisterGameplayTagEvent(USA_CHARACTER_ADJUST_IGNOREROTATETOMOVE, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::GameplayTagIgnoreRotateToMoveCallback);

	ASC->RegisterGameplayTagEvent(USA_CHARACTER_ADJUST_IGNOREMOVEINPUT, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::GameplayTagIgnoreMoveInputCallback);

	ASC->RegisterGameplayTagEvent(USA_CHARACTER_ADJUST_VELOCITYZERO, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::GameplayTagVelocityZeroCallback);

	ASC->RegisterGameplayTagEvent(USA_CHARACTER_ADJUST_CANNOTWALKOFFLEDGE, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::GameplayTagCanNotWalkOffLedgeCallback);


	ASC->RegisterGameplayTagEvent(USA_CHARACTER_ACTION_SLIDE, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::GameplayTagSlideCallback);

	ASC->RegisterGameplayTagEvent(USA_CHARACTER_STATE_CROUCH , EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::GameplayTagCrouchCallback);
}

