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

#include "Weapon/USAWeaponBase.h"


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

void FUSACharacterCapsuleInfo::RenewCharacterCapsule(ACharacter* InCharacter)
{
	if (InCharacter == nullptr)
	{
		return;
	}

	if (InCharacter->GetCapsuleComponent() != nullptr
		&& InCharacter->GetMovementComponent() != nullptr
		/*&& InCharacter->GetCharacterMovement()->IsFalling() == false*/
		/*&& InCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() > CapsuleHaflHeight*/)
	{
		FVector NewLocation = InCharacter->GetActorLocation();

		switch (CapsulePivot)
		{
		case EUSACharacterCapsulePivot::Top:
			NewLocation.Z += -(CapsuleHaflHeight - InCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
			break;

		case EUSACharacterCapsulePivot::Center:
			break;

		case EUSACharacterCapsulePivot::Bottom:
			NewLocation.Z += (CapsuleHaflHeight - InCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
			break;
		}

		InCharacter->SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
	}

	if (InCharacter->GetMesh() != nullptr)
	{
		switch (CapsulePivot)
		{
		case EUSACharacterCapsulePivot::Top:
			InCharacter->GetMesh()->SetRelativeLocation(FVector::UpVector * -(CapsuleOriginalHalfHeight * 2.0f - CapsuleHaflHeight));
			break;

		case EUSACharacterCapsulePivot::Center:
			InCharacter->GetMesh()->SetRelativeLocation(FVector::UpVector * -(CapsuleOriginalHalfHeight));
			break;

		case EUSACharacterCapsulePivot::Bottom:
			InCharacter->GetMesh()->SetRelativeLocation(FVector::UpVector * -(CapsuleHaflHeight));
			break;
		}
	}

	InCharacter->GetCapsuleComponent()->SetCapsuleHalfHeight(CapsuleHaflHeight);
	InCharacter->GetCapsuleComponent()->SetCapsuleRadius(CapsuleRadius);
}


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

	CharacterCapsuleWalkInfo.RenewCharacterCapsule(this);
	CharacterMovementWalkInfo.RenewCharacterMovementInfo(GetCharacterMovement());
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

	//USA_LOG(LogTemp, Log, TEXT("ASC is here"));

	FGameplayAbilitySpec* GameplayAbilitySpec = ASC->FindAbilitySpecFromInputID(InputID);

	if (GameplayAbilitySpec == nullptr)
	{
		return;
	}

	//USA_LOG(LogTemp, Log, TEXT("GameplayAbilitySpec is here"));

	if (GameplayAbilitySpec->IsActive())
	{
		//USA_LOG(LogTemp, Log, TEXT("GameplayAbilitySpec AbilitySpecInputPressed"));

		ASC->AbilitySpecInputPressed(*GameplayAbilitySpec);
	}
	else
	{
		//USA_LOG(LogTemp, Log, TEXT("GameplayAbilitySpec TryActivateAbility"));

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

void AUSACharacterBase::OnGameplayTagCallback_IgnoreRotateToMove(const FGameplayTag CallbackTag, int32 NewCount)
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

void AUSACharacterBase::OnGameplayTagCallback_IgnoreMoveInput(const FGameplayTag CallbackTag, int32 NewCount)
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

void AUSACharacterBase::OnGameplayTagCallback_VelocityZero(const FGameplayTag CallbackTag, int32 NewCount)
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

void AUSACharacterBase::OnGameplayTagCallback_CanNotWalkOffLedge(const FGameplayTag CallbackTag, int32 NewCount)
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

void AUSACharacterBase::OnGameplayTagCallback_Walk(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (GetCharacterMovement() == nullptr)
	{
		return;
	}

	if (NewCount > 0)
	{
		CharacterMovementRealWalkInfo.RenewCharacterMovementInfo(GetCharacterMovement());
	}
	else
	{
		CharacterMovementWalkInfo.RenewCharacterMovementInfo(GetCharacterMovement());
	}
}

void AUSACharacterBase::OnGameplayTagCallback_Fall(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (GetCapsuleComponent() == nullptr)
	{
		return;
	}

	if (NewCount > 0)
	{
		CharacterCapsuleFallInfo.RenewCharacterCapsule(this);
	}
	else
	{
		CharacterCapsuleWalkInfo.RenewCharacterCapsule(this);
	}
}

void AUSACharacterBase::OnGameplayTagCallback_Slide(const FGameplayTag CallbackTag, int32 NewCount)
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

void AUSACharacterBase::OnGameplayTagCallback_Crouch(const FGameplayTag CallbackTag, int32 NewCount)
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

void AUSACharacterBase::OnGameplayTagCallback_HandFirstWeapon(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		//USA_LOG(LogTemp, Log, TEXT("First on Hand"));
		AttachWeaponToHandSocket(CurrentEquipedWeapons[EUSAWeaponType::First]);
	}
	else
	{
		//USA_LOG(LogTemp, Log, TEXT("First on Spine"));
		AttachWeaponToHolderSocket(CurrentEquipedWeapons[EUSAWeaponType::First]);
	}
}

void AUSACharacterBase::OnGameplayTagCallback_HandSecondWeapon(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		AttachWeaponToHandSocket(CurrentEquipedWeapons[EUSAWeaponType::Second]);
	}
	else
	{
		AttachWeaponToHolderSocket(CurrentEquipedWeapons[EUSAWeaponType::Second]);
	}
}

void AUSACharacterBase::EquipWeapon(AUSAWeaponBase* InWeapon)
{
	if (InWeapon == nullptr)
	{
		return;
	}
	
	EUSAWeaponType InWeaponType = InWeapon->GetWeaponType();

	//if (CurrentEquipedWeapons.)
	//{
	//	CurrentEquipedWeapons = new TMap <EUSAWeaponType, AUSAWeaponBase>();
	//}

	if (CurrentEquipedWeapons.Contains (InWeaponType)
		&& CurrentEquipedWeapons[InWeaponType] != nullptr)
	{
		CurrentEquipedWeapons[InWeaponType]->ClearGameplayWeaponAbilitesToASC(GetAbilitySystemComponent());
	}
	else
	{
		CurrentEquipedWeapons.Add({ InWeaponType, nullptr });
	}

	InWeapon->GiveGameplayWeaponAbilitesToASC(GetAbilitySystemComponent());

	AttachWeaponToHolderSocket(InWeapon);

	CurrentEquipedWeapons[InWeaponType] = InWeapon;
}

void AUSACharacterBase::UnequipWeapon(AUSAWeaponBase* InWeapon)
{
	if (InWeapon == nullptr)
	{
		return;
	}

	EUSAWeaponType InWeaponType = InWeapon->GetWeaponType();

	if (CurrentEquipedWeapons.Contains(InWeaponType)
		&& CurrentEquipedWeapons[InWeaponType] != nullptr)
	{
		if (InWeapon == CurrentEquipedWeapons[InWeaponType])
		{
			InWeapon->ClearGameplayWeaponAbilitesToASC(GetAbilitySystemComponent());
			CurrentEquipedWeapons[InWeaponType] = nullptr;
		}
	}

	// TODO: 드롭 혹은 파괴 과정 수행할 것
}

void AUSACharacterBase::AttachWeaponToHandSocket(AUSAWeaponBase* InWeapon)
{
	if (InWeapon == nullptr)
	{
		return;
	}

	if (GetMesh() == nullptr)
	{
		return;
	}

	FAttachmentTransformRules AttachmentTransformRules
	(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);

	InWeapon->AttachToComponent(GetMesh(), AttachmentTransformRules, InWeapon->GetWeaponHandSocketName());
}

void AUSACharacterBase::AttachWeaponToHolderSocket(AUSAWeaponBase* InWeapon)
{
	if (InWeapon == nullptr)
	{
		return;
	}

	if (GetMesh() == nullptr)
	{
		return;
	}

	FAttachmentTransformRules AttachmentTransformRules
	(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);

	InWeapon->AttachToComponent(GetMesh(), AttachmentTransformRules, InWeapon->GetWeaponHolderSocketName());
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

	// 게임 어빌리티 부여
	if (HasAuthority() == true)
	{
		//if (ASC != nullptr)
		//{
		//USA_LOG(LogTemp, Log, TEXT("ASC is here"));

			for (const auto& GameplayTriggerAbility : GameplayTriggerAbilities)
			{
				FGameplayAbilitySpec GameplayAbilitySpec(GameplayTriggerAbility);
				ASC->GiveAbility(GameplayAbilitySpec);
			}

			for (const auto& GameplayActionAbility : GameplayActiveAbilities)
			{
				//USA_LOG(LogTemp, Log, TEXT("Try to give GameplayActiveAbilities... "))

					FGameplayAbilitySpec GameplayAbilityActionSpec(GameplayActionAbility.GameplayAbility);

				if (GameplayActionAbility.InputID >= 0)
				{
					GameplayAbilityActionSpec.InputID = GameplayActionAbility.InputID;
				}

				ASC->GiveAbility(GameplayAbilityActionSpec);

				//USA_LOG(LogTemp, Log, TEXT("Gave GameplayActiveAbilities! "))
			}

			// 게임 시작 어빌리티
			for (const auto& GameplayStartAbility : GameplayStartAbilities)
			{
				FGameplayAbilitySpec GameplayAbilitySpec(GameplayStartAbility);
				ASC->GiveAbility(GameplayStartAbility);
			}

			for (const auto& GameplayStartAbility : GameplayStartAbilities)
			{
				if (GameplayStartAbility == nullptr)
				{
					continue;
				}

				FGameplayAbilitySpec* GameplayAbilitySpec = ASC->FindAbilitySpecFromClass(GameplayStartAbility);

				if (GameplayAbilitySpec == nullptr)
				{
					continue;
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

			//for (const auto& GameplayStartActionAbility : GameplayStartActionAbilites)
			//{
			//	FGameplayAbilitySpec GameplayAbilitySpec(GameplayStartActionAbility);
			//	ASC->GiveAbility(GameplayStartActionAbility);
			//}

			//for (const auto& GameplayStartActionAbility : GameplayStartActionAbilites)
			//{
			//	if (GameplayStartActionAbility == nullptr)
			//	{
			//		continue;
			//	}

			//	FGameplayAbilitySpec* GameplayAbilitySpec = ASC->FindAbilitySpecFromClass(GameplayStartActionAbility);

			//	if (GameplayAbilitySpec == nullptr)
			//	{
			//		continue;
			//	}

			//	if (GameplayAbilitySpec->IsActive())
			//	{
			//		ASC->AbilitySpecInputPressed(*GameplayAbilitySpec);
			//	}
			//	else
			//	{
			//		ASC->TryActivateAbility(GameplayAbilitySpec->Handle);
			//	}
			//}
		//}
	}

	ASC->RegisterGameplayTagEvent(USA_CHARACTER_ADJUST_IGNOREROTATETOMOVE, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::OnGameplayTagCallback_IgnoreRotateToMove);

	ASC->RegisterGameplayTagEvent(USA_CHARACTER_ADJUST_IGNOREMOVEINPUT, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::OnGameplayTagCallback_IgnoreMoveInput);

	ASC->RegisterGameplayTagEvent(USA_CHARACTER_ADJUST_VELOCITYZERO, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::OnGameplayTagCallback_VelocityZero);

	ASC->RegisterGameplayTagEvent(USA_CHARACTER_ADJUST_CANNOTWALKOFFLEDGE, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::OnGameplayTagCallback_CanNotWalkOffLedge);


	ASC->RegisterGameplayTagEvent(USA_CHARACTER_STATE_FALL, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::OnGameplayTagCallback_Fall);

	ASC->RegisterGameplayTagEvent(USA_CHARACTER_STATE_WALK, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::OnGameplayTagCallback_Walk);


	ASC->RegisterGameplayTagEvent(USA_CHARACTER_ACTION_SLIDE, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::OnGameplayTagCallback_Slide);

	ASC->RegisterGameplayTagEvent(USA_CHARACTER_STATE_CROUCH , EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::OnGameplayTagCallback_Crouch);


	ASC->RegisterGameplayTagEvent(USA_CHARACTER_HAND_FIRSTWEAPON, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::OnGameplayTagCallback_HandFirstWeapon);

	ASC->RegisterGameplayTagEvent(USA_CHARACTER_HAND_SECONDWEAPON, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::OnGameplayTagCallback_HandSecondWeapon);
}

