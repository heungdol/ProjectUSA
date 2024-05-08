// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USACharacterBase.h"

#include "GameFramework/DamageType.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerState.h"

#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"

#include "Camera/CameraComponent.h"
#include "Camera/USACameraComponent.h"
#include "Camera/USASpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Component/USACharacterMovementComponent.h"
#include "Component/USAJellyEffectComponent.h"
#include "Component/USACharacterPivotComponent.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

#include "Engine/DamageEvents.h"
#include "Net/UnrealNetwork.h"
#include "Kismet\KismetSystemLibrary.h"

#include "GAS/GA/USAGameplayAbility.h"
#include "GAS/AttributeSet/USAAttributeSet.h"
#include "GameplayEffect.h"

#include "Kismet/GameplayStatics.h"

#include "Weapon/USAWeaponBase.h"

#include "Tag/USAGameplayTags.h"
#include "ProjectUSA.h"


// ====================================================================================


void FUSACharacterCapsuleInfo::RenewCharacterCapsule(ACharacter* InCharacter)
{
	RenewCharacterCapsuleLocation(InCharacter);
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
}

void FUSACharacterCapsuleInfo::RenewCharacterCapsuleLocation(ACharacter* InCharacter)
{
	if (InCharacter == nullptr)
	{
		return;
	}

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


	FVector GroundHitLocation = FVector::ZeroVector;

	if (CharacterMovementComponent->CurrentFloor.bBlockingHit)
	{
		GroundHitLocation = CharacterMovementComponent->CurrentFloor.HitResult.ImpactPoint;
	}

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

	if (CharacterMovementComponent->CurrentFloor.bBlockingHit &&
		GroundHitLocation.Z > NewLocation.Z - CapsuleHaflHeight)
	{
		NewLocation.Z = GroundHitLocation.Z + CapsuleHaflHeight;
	}

	InCharacter->SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);


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
	}

	InCharacter->GetMesh()->SetRelativeLocation(NewUpdatedComponentsLocation);
}


// ====================================================================================


void FUSACharacterAttributeSetInfo::RenewUSACharacterAttributeSetData(UAbilitySystemComponent* InASC)
{
	if (InASC == nullptr)
	{
		return;
	}

	// 아래 구문은 잘 수행시킬 것

	if (InASC->GetSet <UUSAAttributeSet>() != nullptr)
	{
		InASC->SetNumericAttributeBase(UUSAAttributeSet::GetMaxHealthAttribute(), StartMaxHealth);
		InASC->SetNumericAttributeBase(UUSAAttributeSet::GetCurrentHealthAttribute(), StartCurrentHealth);
		InASC->SetNumericAttributeBase(UUSAAttributeSet::GetBaseArmorAttribute(), StartBaseArmor);
		InASC->SetNumericAttributeBase(UUSAAttributeSet::GetCurrentArmorAttribute(), StartBaseArmor);
	}
}


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


	NicknameWidgetComponent = CreateDefaultSubobject <UWidgetComponent>(TEXT("Nickname Widget Component"));
	NicknameWidgetComponent->SetupAttachment(RootComponent);

	HealthBarWidgetComponent = CreateDefaultSubobject <UWidgetComponent>(TEXT("Health Bar Widget Component"));
	HealthBarWidgetComponent->SetupAttachment(RootComponent);


	//PivotComponent = CreateDefaultSubobject <UUSACharacterPivotComponent>(TEXT("Character Pivot Component"));
	//PivotComponent->SetupAttachment(RootComponent);

	//CharacterCapsuleWalkInfo.RenewCharacterCapsule(this);
	//CharacterMovementWalkInfo.RenewCharacterMovementInfo(GetCharacterMovement());

	//GetCharacterMovement()->MovementState.bCanCrouch = true;
	//GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 0.0f;
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;

	ASC = nullptr;

	bIsSetNextWeaponBeforeGASSetup = false;

	//NetUpdateFrequency = 200.0f;
}

//void AUSACharacterBase::OnConstruction(const FTransform& Transform)
//{
//	Super::OnConstruction(Transform);
//
//	//CharacterCapsuleWalkInfo.RenewCharacterCapsule(this);
//	CharacterMovementWalkInfo.RenewCharacterMovementInfo(GetCharacterMovement());
//}

bool AUSACharacterBase::ServerRPC_RenewCharacterCapsule_Validate(ACharacter* InCharacter, const FName& InKeyName)
{
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

	CharacterCapsuleInfos[InKeyName].RenewCharacterCapsule(InCharacter);
}

//

//void AUSACharacterBase::OnRep_HealthBarWidgetComponent()
//{
//	USA_LOG(LogTemp, Log, TEXT("Update HP Bar"));
//}

float AUSACharacterBase::PlayAnimMontage(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	if (GetWorld()->GetNetMode() == ENetMode::NM_Standalone)
	{
		return Super::PlayAnimMontage(AnimMontage, InPlayRate, StartSectionName);
	}

	if (AnimMontage == nullptr)
	{
		return 0.0f;
	}

	float AnimMontageDuration = AnimMontage->GetPlayLength();
	AnimMontageDuration = AnimMontageDuration / (InPlayRate * AnimMontage->RateScale);

	ServerRPC_PlayAnimMontage(AnimMontage, InPlayRate, StartSectionName);

	return AnimMontageDuration;
}

void AUSACharacterBase::ServerRPC_PlayAnimMontage_Implementation(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	MulticastRPC_PlayAnimMontage(AnimMontage, InPlayRate, StartSectionName);
}

void AUSACharacterBase::MulticastRPC_PlayAnimMontage_Implementation(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	Super::PlayAnimMontage(AnimMontage, InPlayRate, StartSectionName);
}

//

void AUSACharacterBase::StopAnimMontage(UAnimMontage* AnimMontage)
{
	if (GetWorld()->GetNetMode() == ENetMode::NM_Standalone)
	{
		Super::StopAnimMontage(AnimMontage);
		return;
	}

	ServerRPC_StopAnimMontage();
}

void AUSACharacterBase::ServerRPC_StopAnimMontage_Implementation(UAnimMontage* AnimMontage)
{
	MulticastRPC_StopAnimMontage(AnimMontage);
}

void AUSACharacterBase::MulticastRPC_StopAnimMontage_Implementation(UAnimMontage* AnimMontage)
{
	Super::StopAnimMontage(AnimMontage);
}

//

void AUSACharacterBase::AddWaitingWeapon(AUSAWeaponBase* InNextWeapon)
{
	//MulticastRPC_SetNextWeapon(InNextWeapon);

	if (UKismetSystemLibrary::IsStandalone(GetWorld())
		|| UKismetSystemLibrary::IsServer(GetWorld()))
	{

	}
	else
	{
		return;
	}
	
	if (InNextWeapon != nullptr)
	{
		//NextWeapon = InNextWeapon;

		//EUSAWeaponType WeaponType = InNextWeapon->GetWeaponType();

		//if (NextWaitingWeapons.Contains(WeaponType)
		//	&& NextWaitingWeapons[WeaponType] != nullptr)
		//{
		//	NextWaitingWeapons[WeaponType]->ClearGameplayWeaponAbilitesToASC(GetAbilitySystemComponent());
		//}
		//else
		//{
		//	NextWaitingWeapons.Add({ WeaponType, nullptr });
		//}

		//NextWaitingWeapons[WeaponType] = InNextWeapon;

		NextWaitingWeapons.Add(InNextWeapon);

		OnRep_NextWeapon();
	}
}

//void AUSACharacterBase::MulticastRPC_SetNextWeapon_Implementation(AUSAWeaponBase* InNextWeapon)
//{
//	//if (HasAuthority())
//	//{
//	//	
//	//}
//}

void AUSACharacterBase::OnRep_NextWeapon()
{
	if (ASC == nullptr)
	{
		bIsSetNextWeaponBeforeGASSetup = true;

		return;
	}

	UpdateCurrentWeapons();

	//EquipFinalNextWeapon();
}

//void AUSACharacterBase::EquipFinalNextWeapon()
//{
//	//EUSAWeaponType WeaponType = EUSAWeaponType::None;
//
//	//if (NextWeapon != nullptr)
//	//{
//	//	WeaponType = NextWeapon->GetWeaponType();
//	//}
//
//	EquipWeapon(NextWeapon);
//}

FVector AUSACharacterBase::GetUSACharacterDirection_InputMovement()
{
	return USACharacterInputMovementDirection;
}

FVector AUSACharacterBase::GetUSACharacterDirection_Target()
{
	// 임시 타겟팅 판단 위함
	bool bIsInstantTargeting = false;

	// 우선 널 포인터라면 임시 타겟팅으로 판단
	if (CurrentTargetableActor == nullptr)
	{
		bIsInstantTargeting = true;
		UpdateCurrentTargetableActor_Instant();
	}
	
	if (CurrentTargetableActor == nullptr)
	{
		return GetUSACharacterDirection_InputMovement();
	}

	if (Cast<AActor>(CurrentTargetableActor) == nullptr)
	{
		return GetUSACharacterDirection_InputMovement();
	}
	
	FVector Result = FVector::ForwardVector;

	FVector SourceLocation = GetActorLocation();
	FVector TargetLocation = Cast<AActor>(CurrentTargetableActor)->GetActorLocation();

	Result = TargetLocation - SourceLocation;
	Result.Z = 0.0f;

	Result.Normalize();

	// 임시 타겟팅인 경우, 원상복구
	if (bIsInstantTargeting)
	{
		CurrentTargetableActor = nullptr;
	}

	return Result;
}

void AUSACharacterBase::DoTarget(const struct FInputActionValue& Value)
{
	// ...
}

//

void AUSACharacterBase::Falling()
{
	Super::Falling();

	if (GetLocalRole() == ENetRole::ROLE_Authority
		|| GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
	{
		ServerRPC_RenewCharacterCapsule(this, KEYNAME_CAPSULEINFO_FALL);
	}
}

void AUSACharacterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (GetLocalRole() == ENetRole::ROLE_Authority
		|| GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
	{
		ServerRPC_RenewCharacterCapsule(this, KEYNAME_CAPSULEINFO_WALK);
	}
}

//

void AUSACharacterBase::OnUSACrouch()
{
	if (GetLocalRole() == ENetRole::ROLE_Authority
		|| GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
	{
		ServerRPC_OnUSACrouch();
	}
}

void AUSACharacterBase::ServerRPC_OnUSACrouch_Implementation()
{
	MulticastRPC_OnUSACrouch();
}

void AUSACharacterBase::MulticastRPC_OnUSACrouch_Implementation()
{
	K2_OnUSACrouch();
}

//

void AUSACharacterBase::OnUSAUnCrouch()
{
	if (GetLocalRole() == ENetRole::ROLE_Authority
		|| GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
	{
		ServerRPC_OnUSAUnCrouch();
	}
}

void AUSACharacterBase::ServerRPC_OnUSAUnCrouch_Implementation()
{
	MulticastRPC_OnUSAUnCrouch();
}

void AUSACharacterBase::MulticastRPC_OnUSAUnCrouch_Implementation()
{
	K2_OnUSAUnCrouch();
}

//

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

	CharacterCapsuleInfos[KEYNAME_CAPSULEINFO_WALK].RenewCharacterCapsule(this);

	//ResetAttributeSet();

	K2_OnCurrentHealthRatioChanged(GetCharacterCurrentHealthRatio_Implementation());
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
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUSACharacterBase::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AUSACharacterBase::MoveEnd);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUSACharacterBase::Look);
		EnhancedInputComponent->BindAction(TargetAction, ETriggerEvent::Triggered, this, &AUSACharacterBase::DoTarget);

		for (const auto& GameplayActiveAbility : GameplayAbilities_Active)
		{
			if (GameplayActiveAbility.InputID < 0)
			{
				continue;
			}

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

	// 서버에서 수행
	SetupGAS();

	PostSetupGAS();

	SetupAttributeSet();

	BeginStartAbilities();

	// 시작할 때 자동으로 콘솔 입력
	APlayerController* PlayerController = Cast <APlayerController>(NewController);
	if (PlayerController != nullptr)
	{
		PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
	}
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

		// 조작감 개선을 위해 사용할 변수
		USACharacterInputMovementDirection = ForwardDirection * MovementVector.Y;
		USACharacterInputMovementDirection += RightDirection * MovementVector.X;
	}
}

void AUSACharacterBase::MoveEnd(const FInputActionValue& Value)
{
	//USA_LOG(LogTemp, Log, TEXT("End Move"));

	//USACharacterInputMovementDirection = FVector::ZeroVector;
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

float AUSACharacterBase::GetCharacterCurrentHealth_Implementation()
{
	float Result = 0.0f;

	UUSAAttributeSet* USAAttributeSet = nullptr;

	if (ASC != nullptr)
	{
		USAAttributeSet = const_cast<UUSAAttributeSet*>(ASC->GetSet<UUSAAttributeSet>());
	}

	if (USAAttributeSet != nullptr)
	{
		Result = USAAttributeSet->GetCurrentHealth();
	}

	return Result;
}

float AUSACharacterBase::GetCharacterMaxHealth_Implementation()
{
	float Result = 1.0f;

	UUSAAttributeSet* USAAttributeSet = nullptr;

	if (ASC != nullptr)
	{
		USAAttributeSet = const_cast<UUSAAttributeSet*>(ASC->GetSet<UUSAAttributeSet>());
	}

	if (USAAttributeSet != nullptr)
	{
		Result = USAAttributeSet->GetMaxHealth();
	}

	return Result;
}

float AUSACharacterBase::GetCharacterCurrentArmor_Implementation()
{
	float Result = 0.0f;

	UUSAAttributeSet* USAAttributeSet = nullptr;

	if (ASC != nullptr)
	{
		USAAttributeSet = const_cast<UUSAAttributeSet*>(ASC->GetSet<UUSAAttributeSet>());
	}

	if (USAAttributeSet != nullptr)
	{
		Result = USAAttributeSet->GetCurrentArmor();
	}

	return Result;
}

float AUSACharacterBase::GetCharacterCurrentHealthRatio_Implementation()
{
	float CurrentHealth = GetCharacterCurrentHealth_Implementation();
	float MaxHealth = GetCharacterMaxHealth_Implementation();

	float Result = CurrentHealth / MaxHealth;
	return Result;
}

//void AUSACharacterBase::OnCurrentHealthChangedCallback(const FOnAttributeChangeData& ChangeData)
//{
//	K2_OnCurrentHealthChanged(ChangeData.NewValue);
//}
//
//void AUSACharacterBase::OnMaxHealthChangedCallback(const FOnAttributeChangeData& ChangeData)
//{
//	K2_OnMaxHealthChanged(ChangeData.NewValue);
//}

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
		//CharacterMovementRealWalkInfo.RenewCharacterMovementInfo(GetCharacterMovement());
	}
	else
	{
		//CharacterMovementWalkInfo.RenewCharacterMovementInfo(GetCharacterMovement());
	}
}

void AUSACharacterBase::OnGameplayTagCallback_Fall(const FGameplayTag CallbackTag, int32 NewCount)
{

}

void AUSACharacterBase::OnGameplayTagCallback_Slide(const FGameplayTag CallbackTag, int32 NewCount)
{

}

void AUSACharacterBase::OnGameplayTagCallback_Crouch(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		OnUSACrouch();

		if (GetLocalRole() == ENetRole::ROLE_Authority
			|| GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
		{
			ServerRPC_RenewCharacterCapsule(this, KEYNAME_CAPSULEINFO_CROUCH);
		}
	}
	else
	{
		OnUSAUnCrouch();

		if (GetMovementComponent()->IsFalling())
		{
			if (GetLocalRole() == ENetRole::ROLE_Authority
				|| GetLocalRole() == ENetRole::ROLE_AutonomousProxy)	
			{
				ServerRPC_RenewCharacterCapsule(this, KEYNAME_CAPSULEINFO_FALL);
			}
		}
		else
		{
			if (GetLocalRole() == ENetRole::ROLE_Authority
				|| GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
			{
				ServerRPC_RenewCharacterCapsule(this, KEYNAME_CAPSULEINFO_WALK);
			}
		}
	}
}


void AUSACharacterBase::OnGameplayTagCallback_Dead(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

		K2_OnUSADeath();
	}
	else
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
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

void AUSACharacterBase::CheckCharacterByGameplayTags()
{
	int32 Count = 0;

	if (ASC != nullptr)
	{
		Count = ASC->GetGameplayTagCount(USA_CHARACTER_STATE_DEAD);
	}

	OnGameplayTagCallback_Dead(USA_CHARACTER_STATE_DEAD, Count);
}

void AUSACharacterBase::UpdateCurrentWeapons(/*AUSAWeaponBase* InWeapon*/)
{
	//if (InWeapon == nullptr)
	//{
	//	return;
	//}

	for (AUSAWeaponBase* NextWeapon : NextWaitingWeapons)
	{
		if (NextWeapon == nullptr)
		{
			continue;
		}

		EUSAWeaponType WeaponType = NextWeapon->GetWeaponType();

		if (CurrentEquipedWeapons.Contains(WeaponType)
			&& CurrentEquipedWeapons[WeaponType] != nullptr)
		{
			CurrentEquipedWeapons[WeaponType]->ClearGameplayWeaponAbilitesToASC(GetAbilitySystemComponent());
		}
		else
		{
			CurrentEquipedWeapons.Add({ WeaponType, nullptr });
		}

		NextWeapon->GiveGameplayWeaponAbilitesToASC(GetAbilitySystemComponent());

		AttachWeaponToHolderSocket(NextWeapon);

		CurrentEquipedWeapons[WeaponType] = NextWeapon;
	}

	NextWaitingWeapons.Empty();
	
	//EUSAWeaponType InWeaponType = InWeapon->GetWeaponType();

	//if (CurrentEquipedWeapons.Contains (InWeaponType)
	//	&& CurrentEquipedWeapons[InWeaponType] != nullptr)
	//{
	//	CurrentEquipedWeapons[InWeaponType]->ClearGameplayWeaponAbilitesToASC(GetAbilitySystemComponent());
	//}
	//else
	//{
	//	CurrentEquipedWeapons.Add({ InWeaponType, nullptr });
	//}

	//InWeapon->GiveGameplayWeaponAbilitesToASC(GetAbilitySystemComponent());

	//AttachWeaponToHolderSocket(InWeapon);

	//CurrentEquipedWeapons[InWeaponType] = InWeapon;

	//USA_LOG(LogTemp, Log, TEXT("Setting Weapon Complete"));
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

bool AUSACharacterBase::GetIsTargetableCurrently()
{
	//return false;

	if (ASC == nullptr)
	{
		return false;
	}

	if (ASC->GetGameplayTagCount(USA_CHARACTER_STATE_DEAD) > 0)
	{
		return false;
	}

	return true;
}

FVector AUSACharacterBase::GetTargetablePivotlocation()
{
	return GetActorLocation();

	//if (GetMesh() == nullptr)
	//{
	//	return GetActorLocation();
	//}

	//if (GetMesh()->DoesSocketExist(TargetablePivotName) == false)
	//{
	//	return GetActorLocation();
	//}

	//FVector ResultLocation = GetMesh()->GetSocketLocation(TargetablePivotName);

	//DrawDebugSphere(GetWorld(), ResultLocation, 15.0f, 8, FColor::Red, false, -1.0f, 0U, 2.0f);

	//return ResultLocation;

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
	float ResultDamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//USA_LOG(LogTemp, Log, TEXT("Taking Damage..."));

	// 데미지
	MulticastRPC_TakeDamage(ResultDamageAmount);

	// 넉백 어빌리티 수행
	ApplyDamageMomentum(ResultDamageAmount, DamageEvent, EventInstigator->GetPawn(), DamageCauser);

	return ResultDamageAmount;
}

void AUSACharacterBase::MulticastRPC_TakeDamage_Implementation(float DamageAmount)
{
	UUSAAttributeSet* USAAttributeSet = nullptr;

	if (ASC != nullptr)
	{
		USAAttributeSet = const_cast<UUSAAttributeSet*>(ASC->GetSet<UUSAAttributeSet>());
	}

	if (USAAttributeSet != nullptr)
	{
		USAAttributeSet->SetDamage(DamageAmount);
	}

	// 죽음인 경우 델리게이트로 처리
}


void AUSACharacterBase::ApplyDamageMomentum(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
{
	if (ASC == nullptr)
	{
		return;
	}

	if (ASC->HasMatchingGameplayTag(USA_CHARACTER_STATE_DEAD))
	{
		return;
	}

	float CheckCurrentHealth = 0.0f;
	bool CheckIsAttributeFound = false;

	bool bIsPlayingDeathAbility = false;

	CheckCurrentHealth = ASC->GetGameplayAttributeValue(UUSAAttributeSet::GetCurrentHealthAttribute(), CheckIsAttributeFound);

	if (CheckIsAttributeFound == true 
		&& CheckCurrentHealth <= 0.0f)
	{
		bIsPlayingDeathAbility = true;
	}

	FVector NewDirection = FVector::ForwardVector;
	TSubclassOf<UGameplayAbility> DamageAbilityClass;

	FVector AttackDirection;
	FHitResult HitResult;
	TSubclassOf<UDamageType> DamageType;

	DamageEvent.GetBestHitInfo(nullptr, nullptr, HitResult, AttackDirection);

	NewDirection = AttackDirection * -1.0f;
	NewDirection.Z = 0.0f;
	NewDirection.Normalize();

	DamageType = DamageEvent.DamageTypeClass;

	if (bIsPlayingDeathAbility)
	{
		if (GameplayAbilities_Death.Contains(DamageType))
		{
			DamageAbilityClass = GameplayAbilities_Death[DamageType];
		}
	}
	else if (GetMovementComponent()->IsFalling())
	{
		if (GameplayAbilities_DamageGround.Contains(DamageType))
		{
			DamageAbilityClass = GameplayAbilities_DamageAir[DamageType];
		}
	}
	else
	{
		if (GameplayAbilities_DamageGround.Contains(DamageType))
		{
			DamageAbilityClass = GameplayAbilities_DamageGround[DamageType];
		}
	}

	if (GetLocalRole() == ENetRole::ROLE_Authority
		|| GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
	{
		ServerRPC_ApplyDamageMomentum(NewDirection, DamageAbilityClass);
	}
}

void AUSACharacterBase::UpdateCurrentTargetableActor()
{
	// ...
}

void AUSACharacterBase::UpdateCurrentTargetableActor_Instant()
{	
	UpdateCurrentTargetableActor();
}

//void AUSACharacterBase::SetCurrentTargetableActorNullptr()
//{
//	CurrentTargetableActor = nullptr;
//}

bool AUSACharacterBase::ServerRPC_ApplyDamageMomentum_Validate
(const FVector& InNewDirection, TSubclassOf<UGameplayAbility> InAbility)
{
	return true;
}

void AUSACharacterBase::ServerRPC_ApplyDamageMomentum_Implementation
(const FVector& InNewDirection, TSubclassOf<UGameplayAbility> InAbility)
{
	MulticastRPC_ApplyDamageMomentum(InNewDirection, InAbility);
}

void AUSACharacterBase::MulticastRPC_ApplyDamageMomentum_Implementation
(const FVector& InNewDirection, TSubclassOf<UGameplayAbility> InAbility)
{
	if (ASC == nullptr)
	{
		return;
	}

	SetActorRotation(InNewDirection.Rotation());

	if (/*HasAuthority()*/ UKismetSystemLibrary::IsServer(GetWorld()))
	{
		FGameplayAbilitySpec* GameplayAbilitySpec = ASC->FindAbilitySpecFromClass(InAbility);

		if (GameplayAbilitySpec != nullptr)
		{
			ASC->TryActivateAbility(GameplayAbilitySpec->Handle);
		}
	}
}

UAbilitySystemComponent* AUSACharacterBase::GetAbilitySystemComponent() const
{
	return ASC;
}

void AUSACharacterBase::OnRep_ASC()
{	
	// ... 
}

void AUSACharacterBase::OnRep_bIsASCInitialized()
{
	// ...
}

void AUSACharacterBase::OnCurrentHealthRatioChanged(float InValue)
{
	K2_OnCurrentHealthRatioChanged(GetCharacterCurrentHealthRatio_Implementation());
}

void AUSACharacterBase::OnCurrentHealthRatioChanged(const FOnAttributeChangeData& ChangeData)
{
	K2_OnCurrentHealthRatioChanged(GetCharacterCurrentHealthRatio_Implementation());
}

void AUSACharacterBase::SetupGAS()
{
	// ...

	bIsASCInitialized = true;
}

void AUSACharacterBase::PostSetupGAS()
{
	if (ASC == nullptr)
	{
		return;
	}

	// 게임 어빌리티 부여
	if (HasAuthority() == true)
	{
		for (const auto& GameplayTriggerAbility : GameplayAbilities_Trigger)
		{
			FGameplayAbilitySpec GameplayAbilitySpec(GameplayTriggerAbility);
			ASC->GiveAbility(GameplayAbilitySpec);
		}

		for (const auto& GameplayActionAbility : GameplayAbilities_Active)
		{
			FGameplayAbilitySpec GameplayAbilityActionSpec(GameplayActionAbility.GameplayAbility);

			if (GameplayActionAbility.InputID >= 0)
			{
				GameplayAbilityActionSpec.InputID = GameplayActionAbility.InputID;
			}

			ASC->GiveAbility(GameplayAbilityActionSpec);
		}

		// 게임 시작 어빌리티
		for (const auto& GameplayStartAbility : GameplayAbilities_Start)
		{
			FGameplayAbilitySpec GameplayAbilitySpec(GameplayStartAbility);
			ASC->GiveAbility(GameplayStartAbility);
		}

		// 데미지 어빌리티
		for (const auto& GameplayDamageAbility : GameplayAbilities_DamageGround)
		{
			FGameplayAbilitySpec GameplayAbilitySpec(GameplayDamageAbility.Value);
			ASC->GiveAbility(GameplayAbilitySpec);
		}

		for (const auto& GameplayDamageAbility : GameplayAbilities_DamageAir)
		{
			FGameplayAbilitySpec GameplayAbilitySpec(GameplayDamageAbility.Value);
			ASC->GiveAbility(GameplayAbilitySpec);
		}

		// 죽음 어빌리티
		for (const auto& GameplayTriggerAbility : GameplayAbilities_Death)
		{
			FGameplayAbilitySpec GameplayAbilitySpec(GameplayTriggerAbility.Value);
			ASC->GiveAbility(GameplayAbilitySpec);
		}

		// 기타 어빌리티
		for (const auto& GameplayTriggerAbility : GameplayAbilities_ETC)
		{
			FGameplayAbilitySpec GameplayAbilitySpec(GameplayTriggerAbility);
			ASC->GiveAbility(GameplayAbilitySpec);
		}

		// 첫 스폰 시, 딜레이를 고려한 무기 장착 과정
		if (bIsSetNextWeaponBeforeGASSetup)
		{
			bIsSetNextWeaponBeforeGASSetup = false;

			//EquipFinalNextWeapon();

			UpdateCurrentWeapons();
		}
	}



	RegisteredGameplayTagEvents.Add
	(
		USA_CHARACTER_ADJUST_IGNOREROTATETOMOVE,
		ASC->RegisterGameplayTagEvent(USA_CHARACTER_ADJUST_IGNOREROTATETOMOVE, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::OnGameplayTagCallback_IgnoreRotateToMove)
	);

	RegisteredGameplayTagEvents.Add
	(
		USA_CHARACTER_ADJUST_IGNOREMOVEINPUT,
		ASC->RegisterGameplayTagEvent(USA_CHARACTER_ADJUST_IGNOREMOVEINPUT, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::OnGameplayTagCallback_IgnoreMoveInput)
	);

	RegisteredGameplayTagEvents.Add
	(
		USA_CHARACTER_ADJUST_VELOCITYZERO,
		ASC->RegisterGameplayTagEvent(USA_CHARACTER_ADJUST_VELOCITYZERO, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::OnGameplayTagCallback_VelocityZero)
	);

	RegisteredGameplayTagEvents.Add
	(
		USA_CHARACTER_ADJUST_CANNOTWALKOFFLEDGE,
		ASC->RegisterGameplayTagEvent(USA_CHARACTER_ADJUST_CANNOTWALKOFFLEDGE, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::OnGameplayTagCallback_CanNotWalkOffLedge)
	);

	//

	RegisteredGameplayTagEvents.Add
	(
		USA_CHARACTER_STATE_WALK,
		ASC->RegisterGameplayTagEvent(USA_CHARACTER_STATE_WALK, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::OnGameplayTagCallback_Walk)
	);

	//

	RegisteredGameplayTagEvents.Add
	(
		USA_CHARACTER_STATE_FALL,
		ASC->RegisterGameplayTagEvent(USA_CHARACTER_STATE_FALL, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::OnGameplayTagCallback_Fall)
	);

	RegisteredGameplayTagEvents.Add
	(
		USA_CHARACTER_ACTION_SLIDE,
		ASC->RegisterGameplayTagEvent(USA_CHARACTER_ACTION_SLIDE, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::OnGameplayTagCallback_Slide)
	);

	RegisteredGameplayTagEvents.Add
	(
		USA_CHARACTER_STATE_CROUCH,
		ASC->RegisterGameplayTagEvent(USA_CHARACTER_STATE_CROUCH, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::OnGameplayTagCallback_Crouch)
	);

	RegisteredGameplayTagEvents.Add
	(
		USA_CHARACTER_STATE_DEAD,
		ASC->RegisterGameplayTagEvent(USA_CHARACTER_STATE_DEAD, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::OnGameplayTagCallback_Dead)
	);

	//

	RegisteredGameplayTagEvents.Add
	(
		USA_CHARACTER_HAND_FIRSTWEAPON,
		ASC->RegisterGameplayTagEvent(USA_CHARACTER_HAND_FIRSTWEAPON, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::OnGameplayTagCallback_HandFirstWeapon)
	);

	RegisteredGameplayTagEvents.Add
	(
		USA_CHARACTER_HAND_SECONDWEAPON,
		ASC->RegisterGameplayTagEvent(USA_CHARACTER_HAND_SECONDWEAPON, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::OnGameplayTagCallback_HandSecondWeapon)
	);
}

void AUSACharacterBase::BeginStartAbilities()
{
	if (ASC != nullptr)
	{
		for (const auto& GameplayStartAbility : GameplayAbilities_Start)
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

void AUSACharacterBase::SetupAttributeSet()
{
	//if (GetLocalRole() != nullptr)
	//{
	//	GetNetOwningPlayer()->GetLocalRole();
	//}

	//GetLocalRole();

	// 어트리뷰트 설정
	if (ASC != nullptr)
	{
		if (ASC->GetSet <UUSAAttributeSet>() != nullptr)
		{
			ASC->GetGameplayAttributeValueChangeDelegate(UUSAAttributeSet::GetCurrentHealthAttribute()).AddUObject
			(this, &AUSACharacterBase::OnCurrentHealthRatioChanged);

			ASC->GetGameplayAttributeValueChangeDelegate(UUSAAttributeSet::GetMaxHealthAttribute()).AddUObject
			(this, &AUSACharacterBase::OnCurrentHealthRatioChanged);

			//ASC->GetSet <UUSAAttributeSet>()->OnOutOfHealth.AddDynamic(this, &AUSACharacterBase::DieUSACharacter);

			//ASC->GetSet <UUSAAttributeSet>()->OnRevive

			ASC->GetSet <UUSAAttributeSet>()->OnCurrentHealthChanged.AddDynamic
			(this, &AUSACharacterBase::OnCurrentHealthRatioChanged);

			ASC->GetSet <UUSAAttributeSet>()->OnMaxHealthChanged.AddDynamic
			(this, &AUSACharacterBase::OnCurrentHealthRatioChanged);

			//ResetAttributeSet();
		}
	}
}

void AUSACharacterBase::ResetAttributeSet()
{
	// 어트리뷰트 설정
	CharacterAttributeSetInfo.RenewUSACharacterAttributeSetData(ASC);
}



void AUSACharacterBase::DieUSACharacter()
{
	ServerRPC_OnUSADeath();
}

//void AUSACharacterBase::OnUSADeath()
//{
//	
//}

void AUSACharacterBase::ServerRPC_OnUSADeath_Implementation()
{
	MulticastRPC_OnUSADeath();
}

void AUSACharacterBase::MulticastRPC_OnUSADeath_Implementation()
{
	
}


void AUSACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUSACharacterBase, ASC);
	DOREPLIFETIME(AUSACharacterBase, bIsASCInitialized);

	DOREPLIFETIME(AUSACharacterBase, NextWaitingWeapons);
}

