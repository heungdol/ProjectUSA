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

#include "GAS/GA/USAGameplayAbility.h"

#include "GameFramework/PlayerState.h"

#include "Component/USAJellyEffectComponent.h"

#include "Tag/USAGameplayTags.h"


#include "Components/SkeletalMeshComponent.h"

#include "Weapon/USAWeaponBase.h"


#include "Engine/DamageEvents.h"
#include "GameFramework/DamageType.h"

#include "Component/USACharacterPivotComponent.h"

#include "Net/UnrealNetwork.h"

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

void FUSACharacterCapsuleInfo::RenewCharacterCapsuleSize(ACharacter* InCharacter)
{
	if (InCharacter == nullptr)
	{
		return;
	}


	InCharacter->GetCapsuleComponent()->SetCapsuleHalfHeight(CapsuleHaflHeight);
	InCharacter->GetCapsuleComponent()->SetCapsuleRadius(CapsuleRadius);

	FVector NewLocation = FVector::ZeroVector;

	if (InCharacter->GetMesh() != nullptr)
	{
		switch (CapsulePivot)
		{
		case EUSACharacterCapsulePivot::Top:
			NewLocation = FVector::UpVector * -(CapsuleOriginalHalfHeight * 2.0f - CapsuleHaflHeight);
			//InCharacter->GetMesh()->SetRelativeLocation(FVector::UpVector * -(CapsuleOriginalHalfHeight * 2.0f - CapsuleHaflHeight));
			break;

		case EUSACharacterCapsulePivot::Center:
			NewLocation = FVector::UpVector * -(CapsuleOriginalHalfHeight);
			//InCharacter->GetMesh()->SetRelativeLocation(FVector::UpVector * -(CapsuleOriginalHalfHeight));
			break;

		case EUSACharacterCapsulePivot::Bottom:
			NewLocation = FVector::UpVector * -(CapsuleHaflHeight);
			//InCharacter->GetMesh()->SetRelativeLocation(FVector::UpVector * -(CapsuleHaflHeight));
			break;
		}

		InCharacter->GetMesh()->SetRelativeLocation(NewLocation);
	}
}

void FUSACharacterCapsuleInfo::RenewCharacterCapsuleLocation(ACharacter* InCharacter)
{
	if (InCharacter == nullptr)
	{
		return;
	}

	if (InCharacter->GetCapsuleComponent() != nullptr
		&& InCharacter->GetMovementComponent() != nullptr)
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

	PivotComponent = CreateDefaultSubobject <UUSACharacterPivotComponent>(TEXT("Character Pivot Component"));
	PivotComponent->SetupAttachment(RootComponent);

	//CharacterCapsuleWalkInfo.RenewCharacterCapsule(this);
	CharacterMovementWalkInfo.RenewCharacterMovementInfo(GetCharacterMovement());

	//GetCharacterMovement()->MovementState.bCanCrouch = true;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 0.0f;
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;

	ASC = nullptr;

	bIsSetNextWeaponBeforeGASSetup = false;

	//NetUpdateFrequency = 200.0f;
}

void AUSACharacterBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	//CharacterCapsuleWalkInfo.RenewCharacterCapsule(this);
	CharacterMovementWalkInfo.RenewCharacterMovementInfo(GetCharacterMovement());
}

bool AUSACharacterBase::ServerRPC_RenewCharacterCapsule_Validate(ACharacter* InCharacter, const FName& InKeyName)
{
	//if (InCharacter == nullptr)
	//{
	//	return false;
	//}

	return true;
}

void AUSACharacterBase::ServerRPC_RenewCharacterCapsule_Implementation(ACharacter* InCharacter, const FName& InKeyName)
{
	MulticastRPC_RenewCharacterCapsule(InCharacter, InKeyName);
}

void AUSACharacterBase::MulticastRPC_RenewCharacterCapsule_Implementation(ACharacter* InCharacter, const FName& InKeyName)
{
	if (InCharacter == nullptr)
	{
		return;
	}

	if (CharacterCapsuleInfos.Num() <= 0)
	{
		return;
	}

	if (CharacterCapsuleInfos.Contains(InKeyName) == false)
	{
		return;
	}

	CharacterCapsuleInfos[InKeyName].RenewCharacterCapsuleSize(InCharacter);
	CharacterCapsuleInfos[InKeyName].RenewCharacterCapsuleLocation(InCharacter);
	
	//if (HasAuthority())
	//{
	//}
}

void AUSACharacterBase::SetNextWeapon(AUSAWeaponBase* InNextWeapon)
{
	MulticastRPC_SetNextWeapon(InNextWeapon);
}

//bool AUSACharacterBase::ServerRPC_SetNextWeapon_Validate(AUSAWeaponBase* InNextWeapon)
//{
//	//USA_LOG(LogTemp, Log, TEXT("Next Weapon Check Chanagable..."));
//
//	//if (InNextWeapon == nullptr)
//	//{
//	//	return false;
//	//}
//
//	return true;
//}
//
//void AUSACharacterBase::ServerRPC_SetNextWeapon_Implementation(AUSAWeaponBase* InNextWeapon)
//{
//	MulticastRPC_SetNextWeapon(InNextWeapon);
//}

void AUSACharacterBase::MulticastRPC_SetNextWeapon_Implementation(AUSAWeaponBase* InNextWeapon)
{
	if (HasAuthority())
	{
		if (InNextWeapon != nullptr)
		{
			NextWeapon = InNextWeapon;
			OnRep_NextWeapon ();
		}
	}
	else
	{

	}
}


void AUSACharacterBase::OnRep_NextWeapon()
{
	USA_LOG(LogTemp, Log, TEXT("Next Weapon Start Chancing"));

	if (ASC == nullptr)
	{
		bIsSetNextWeaponBeforeGASSetup = true;
	
		USA_LOG(LogTemp, Log, TEXT("Put off Setting Next Weapon..."));

		return;
	}

	EquipFinalNextWeapon();

	USA_LOG(LogTemp, Log, TEXT("Next Weapon Change Complete"));
}

void AUSACharacterBase::EquipFinalNextWeapon()
{
	USA_LOG(LogTemp, Log, TEXT("Setting Final Weapon...."));

	EUSAWeaponType WeaponType = EUSAWeaponType::None;

	if (NextWeapon != nullptr)
	{
		WeaponType = NextWeapon->GetWeaponType();
	}

	EquipWeapon(NextWeapon);
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

	CharacterCapsuleInfos[KEYNAME_CAPSULEINFO_WALK].RenewCharacterCapsuleSize(this);
	CharacterCapsuleInfos[KEYNAME_CAPSULEINFO_WALK].RenewCharacterCapsuleLocation(this);

	CharacterMovementWalkInfo.RenewCharacterMovementInfo(GetCharacterMovement());
}

// Called every frame
void AUSACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//AdjustVelocityWithVelocityZero();

	//if (bIsFixRotation)
	//{
	//	SetActorRotation(FixRotation);
	//}
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

	BeginStartAbilities();

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

	//if (NewCount == 1)
	//{
	//	bIsFixRotation = true;
	//	FixRotation = GetActorRotation();
	//}
	//else if (NewCount == 0)
	//{
	//	bIsFixRotation = false;
	//}
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
		//bIsVelocityZero = true;
		GetCharacterMovement()->bApplyGravityWhileJumping = false;
	}
	else
	{
		//bIsVelocityZero = false;
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
		//CharacterCapsuleFallInfo.RenewCharacterCapsule(this);
		ServerRPC_RenewCharacterCapsule(this, KEYNAME_CAPSULEINFO_FALL);
	}
	else
	{
		//CharacterCapsuleWalkInfo.RenewCharacterCapsule(this);
		ServerRPC_RenewCharacterCapsule(this, KEYNAME_CAPSULEINFO_WALK);
		
		//if (bIsCrouched)
		//{

		//}
		//else
		//{
		//}
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
		CharacterMovementSlideInfo.RenewCharacterMovementInfo(GetCharacterMovement());
	}
	else
	{
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

		//if (GetMovementComponent()->IsFalling())
		//{
		//	ServerRPC_RenewCharacterCapsule(this, KEYNAME_CAPSULEINFO_FALL);
		//}
		//else
		//{
		//	ServerRPC_RenewCharacterCapsule(this, KEYNAME_CAPSULEINFO_WALK);
		//}
	}
}

void AUSACharacterBase::OnGameplayTagCallback_HandFirstWeapon(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (CurrentEquipedWeapons.Contains(EUSAWeaponType::First) == false)
	{
		return;
	}

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
	if (CurrentEquipedWeapons.Contains(EUSAWeaponType::Second) == false)
	{
		return;
	}

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

	USA_LOG(LogTemp, Log, TEXT("Setting Weapon Complete"));
}

void AUSACharacterBase::UnequipWeapon(/*AUSAWeaponBase* InWeapon*/EUSAWeaponType InWeaponType)
{
	if (CurrentEquipedWeapons.Contains(InWeaponType)
		&& CurrentEquipedWeapons[InWeaponType] != nullptr)
	{
		CurrentEquipedWeapons[InWeaponType]->ClearGameplayWeaponAbilitesToASC(GetAbilitySystemComponent());
		CurrentEquipedWeapons[InWeaponType] = nullptr;
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
//void AUSACharacterBase::AdjustVelocityWithVelocityZero()
//{
	//if (bIsVelocityZero)
	//{
	//	FVector NewVelocity = GetCharacterMovement()->Velocity;
	//	NewVelocity.X = 0;
	//	NewVelocity.Y = 0;

	//	if (NewVelocity.Z > 0)
	//	{
	//		NewVelocity.Z = 0;
	//	}
	//	else
	//	{
	//		NewVelocity.Z = -NewVelocity.Z;
	//	}

	//	GetCharacterMovement()->Velocity = NewVelocity;
	//	GetCharacterMovement()->UpdateComponentVelocity();
	//}
//}

float AUSACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Result = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ApplyDamageMomentum(DamageAmount, DamageEvent, EventInstigator->GetPawn(), DamageCauser);
	
	return Result;
}

//void AUSACharacterBase::MulticastRPC_ApplyDamageMomentum_Implementation(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
//{
//	if (HasAuthority())
//	{
//
//	}
//	else
//	{
//
//	}
//}



void AUSACharacterBase::ApplyDamageMomentum(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
{
	FVector NewDirection = FVector::ForwardVector;
	TSubclassOf<UGameplayAbility> DamageAbilityClass;

	if (HasAuthority())
	{
		FVector AttackDirection;
		FHitResult HitResult;

		DamageEvent.GetBestHitInfo(nullptr, nullptr, HitResult, AttackDirection);

		NewDirection = AttackDirection * -1.0f;
		NewDirection.Z = 0.0f;
		NewDirection.Normalize();

		
		for (const auto& GameplayDamageAbility : GameplayDamageAbilities)
		{
			if (GameplayDamageAbility.Key == DamageEvent.DamageTypeClass)
			{
				DamageAbilityClass = GameplayDamageAbility.Value;
				//FGameplayAbilitySpec* GameplayAbilitySpec = ASC->FindAbilitySpecFromClass(DamageAbilityClass);
				//
				//if (ASC != nullptr)
				//{
				//	ASC->TryActivateAbility(GameplayAbilitySpec->Handle);
				//}

				break;
			}
		}


		SetActorRotation(NewDirection.Rotation());

		if (DamageAbilityClass != nullptr
			&& ASC != nullptr)
		{
			USA_LOG(LogTemp, Log, TEXT("From"));

			ASC->TryActivateAbilityByClass(DamageAbilityClass);
		}


		if (GetNetMode() != ENetMode::NM_Standalone)
		{
			MulticastRPC_ApplyDamageMomentum(NewDirection, DamageAbilityClass);
		}
	}
}

void AUSACharacterBase::MulticastRPC_ApplyDamageMomentum_Implementation(const FVector& InNewDirection, TSubclassOf<UGameplayAbility> InAbility)
{
	SetActorRotation(InNewDirection.Rotation());

	//if (InAbility != nullptr
	//	&& ASC != nullptr)
	//{
	//	USA_LOG(LogTemp, Log, TEXT ("From"));

	//	//FGameplayAbilitySpec* GameplayAbilitySpec = ASC->FindAbilitySpecFromClass(InAbility);
	//	ASC->TryActivateAbilityByClass(InAbility);
	//}
}

UAbilitySystemComponent* AUSACharacterBase::GetAbilitySystemComponent() const
{
	return ASC;
}

void AUSACharacterBase::OnRep_ASC()
{
	BeginStartAbilities();
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

		// 데미지 어빌리티
		for (const auto& GameplayDamageAbility : GameplayDamageAbilities)
		{
			FGameplayAbilitySpec GameplayAbilitySpec(GameplayDamageAbility.Value);
			ASC->GiveAbility(GameplayAbilitySpec);
		}

		if (bIsSetNextWeaponBeforeGASSetup)
		{
			bIsSetNextWeaponBeforeGASSetup = false;

			EquipFinalNextWeapon();
		}
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

	//USA_LOG(LogTemp, Log, TEXT("My ASC Name is %s %s"), *ASC->GetOwnerActor()->GetName(), *ASC->GetAvatarActor()->GetName());
	//USA_LOG(LogTemp, Log, TEXT("... And My ASC Authority %i"), HasAuthorityOrPredictionKey ();
}

void AUSACharacterBase::BeginStartAbilities()
{
	if (ASC != nullptr)
	{
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
	}
}


void AUSACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUSACharacterBase, ASC);
	DOREPLIFETIME(AUSACharacterBase, NextWeapon);
}