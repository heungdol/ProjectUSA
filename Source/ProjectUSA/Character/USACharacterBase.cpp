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
	GetCapsuleComponent()->InitCapsuleSize(CapsuleRadius, CapsuleHeight * 0.5f);
	//GetCapsuleComponent()->SetRelativeLocation(FVector(0, 0, CapsuleHeight * 0.5f));

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));
	GetMesh()->SetRelativeLocation(FVector(0, 0, CapsuleHeight * -0.5f));

	JellyEffectComponent = CreateDefaultSubobject <UUSAJellyEffectComponent>(TEXT("Jelly Effect Component"));
	JellyEffectComponent->SetMeshComponent(GetMesh());

	ASC = nullptr;
}

void AUSACharacterBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	GetCapsuleComponent()->InitCapsuleSize(CapsuleRadius, CapsuleHeight*0.5f);

	GetMesh()->SetRelativeLocation(FVector(0, 0, CapsuleHeight * -0.5f));
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

	// 게임 시작 어빌리티
	for (const auto& GameplayStartAbility : GameplayStartAbilities)
	{
		//FGameplayAbilitySpec GameplayAbilitySpec(GameplayStartAbility);
		//ASC->TryActivateAbility


		FGameplayAbilitySpec* GameplayAbilitySpec = ASC->FindAbilitySpecFromClass(GameplayStartAbility);

		if (GameplayAbilitySpec == nullptr)
		{
			return;
		}

		//UE_LOG(LogTemp, Log, TEXT("%s"), *GameplayStartAbility->GetName());

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

// Called every frame
void AUSACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUSACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUSACharacterBase::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUSACharacterBase::Look);


		for (const auto& GameplayActiveAbility : GameplayActiveAbilities)
		{
			if (GameplayActiveAbility.InputID < 0)
			{
				continue;
			}

			//UE_LOG(LogTemp, Log, TEXT("GameplayActiveAbility Binding... %s"), *GameplayActiveAbility.InputAction->GetName());

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

	SetupGAS();
}

void AUSACharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

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

	//UE_LOG(LogTemp, Log, TEXT("Input Action %i : ASC is null?"), InputID);

	if (ASC == nullptr)
	{
		return;
	}

	//UE_LOG(LogTemp, Log, TEXT("Input Action %i : Spec is null?"), InputID);

	FGameplayAbilitySpec* GameplayAbilitySpec = ASC->FindAbilitySpecFromInputID(InputID);

	if (GameplayAbilitySpec == nullptr)
	{
		return;
	}

	//UE_LOG(LogTemp, Log, TEXT("Input Action %i"), InputID);

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

void AUSACharacterBase::GameplayTagSlideCallback(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (GetController() == nullptr)
	{
		return;
	}

	if (NewCount > 0)
	{

	}
	else
	{

	}
}

//void AUSACharacterBase::AdjustCharacterInfoByGameplayTagThatASCHas()
//{
//	UE_LOG(LogTemp, Log, TEXT("Gameplay Tag Added or Removed"));
//}
//
//void AUSACharacterBase::AdjustCharacterInfoByGameplayTagThatASCHas(UGameplayAbility* GameplayAbility)
//{
//	AdjustCharacterInfoByGameplayTagThatASCHas();
//}
//
//void AUSACharacterBase::AdjustCharacterInfoByGameplayTagThatASCHas(const FGameplayAbilitySpec& GameplayAbilitySpec)
//{
//	AdjustCharacterInfoByGameplayTagThatASCHas();
//}
//
//void AUSACharacterBase::AdjustCharacterInfoByGameplayTagThatASCHas
//(UAbilitySystemComponent* InASC, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
//{
//	AdjustCharacterInfoByGameplayTagThatASCHas();
//}

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

	// 서버에서만 수행
	if (HasAuthority() == true)
	{
		// 게임 시작 어빌리티
		for (const auto& GameplayStartAbility : GameplayStartAbilities)
		{
			FGameplayAbilitySpec GameplayAbilitySpec(GameplayStartAbility);
			ASC->GiveAbility(GameplayStartAbility);
		}

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
	}

	//ASC->GenericLocalConfirmCallbacks.AddDynamic(this, &AUSACharacterBase::AdjustCharacterInfoByGameplayTagThatASCHas);
	//ASC->GenericLocalCancelCallbacks.AddDynamic(this, &AUSACharacterBase::AdjustCharacterInfoByGameplayTagThatASCHas);

	//ASC->AbilityEndedCallbacks.AddUObject(this, &AUSACharacterBase::AdjustCharacterInfoByGameplayTagThatASCHas);


	//ASC->OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &AUSACharacterBase::AdjustCharacterInfoByGameplayTagThatASCHas);
	//ASC->OnGameplayEffectAppliedDelegateToTarget.AddUObject(this, &AUSACharacterBase::AdjustCharacterInfoByGameplayTagThatASCHas);
	//ASC->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &AUSACharacterBase::AdjustCharacterInfoByGameplayTagThatASCHas);
	//ASC->OnPeriodicGameplayEffectExecuteDelegateOnSelf.AddUObject(this, &AUSACharacterBase::AdjustCharacterInfoByGameplayTagThatASCHas);
	//ASC->OnPeriodicGameplayEffectExecuteDelegateOnTarget.AddUObject(this, &AUSACharacterBase::AdjustCharacterInfoByGameplayTagThatASCHas);


	ASC->RegisterGameplayTagEvent(USA_CHARACTER_STATE_FIXROTATION, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::GameplayTagIgnoreRotateToMoveCallback);

	ASC->RegisterGameplayTagEvent(USA_CHARACTER_STATE_IGNOREMOVEINPUT, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::GameplayTagIgnoreMoveInputCallback);
}


