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
#include "Components/BoxComponent.h"

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
#include "Component/USACharacterAttackComponent.h"

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

#include "Item/USAItemBase.h"

#include "Tag/USAGameplayTags.h"
#include "GameInstance/USAGameInstance.h"
#include "ProjectUSA.h"

#include "AIController.h"
#include "GameFramework/PlayerController.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"

//void AUSACharacterBase::SetWeaponDetectBoxComponentActive(bool InActive)
//{
//	ServerRPC_SetWeaponDetectBoxComponentActive(InActive);
//}
//
//void AUSACharacterBase::ServerRPC_SetWeaponDetectBoxComponentActive_Implementation(bool InActive)
//{
//	MulticastRPC_SetWeaponDetectBoxComponentActive(InActive);
//}
//
//void AUSACharacterBase::MulticastRPC_SetWeaponDetectBoxComponentActive_Implementation(bool InActive)
//{
//	if (IsValid(GetCapsuleComponent()) == true)
//	{
//		//USA_LOG(LogTemp, Log, TEXT("A"));
//		GetCapsuleComponent()->SetGenerateOverlapEvents(InActive);
//	}
//}

//void AUSACharacterBase::AddStartWeapon(AUSAWeaponBase* InWeapon)
//{
//	StartWeapons.Add(InWeapon);
//
//	//if (UKismetSystemLibrary::IsServer(GetWorld()) == true)
//	//{
//	//	OnRep_StartWeapons ();
//	//}
//}
//
//void AUSACharacterBase::UpdateCurrentWeaponsFromStart()
//{
//	//if (IsValid(ASC) == false)
//	//{
//	//	return;
//	//}
//
//	//if (bIsStartWeaponsInitted == true)
//	//{
//	//	return;
//	//}
//
//
//	//bIsStartWeaponsInitted = true;
//}

void AUSACharacterBase::OnRep_CharacterName()
{
	K2_OnCharacterNameChanged();
}

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
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = false;

	CameraSpringArmComponent = CreateDefaultSubobject <UUSASpringArmComponent>(TEXT("Camera Spring Arm Component"));
	CameraSpringArmComponent->SetupAttachment(RootComponent);
	CameraSpringArmComponent->TargetArmLength = 500.0f;
	CameraSpringArmComponent->bUsePawnControlRotation = true;
	CameraSpringArmComponent->SetRelativeLocation(FVector(0, 0, 20.0f));

	CameraComponent = CreateDefaultSubobject <UUSACameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(CameraSpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AUSACharacterBase::OnPickableDetectBoxOverlapBegin);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

	
	//GetMesh()->bSimulationUpdatesChildTransforms = true;
	//GetMesh()->SetOwner
	
	JellyEffectComponent = CreateDefaultSubobject <UUSAJellyEffectComponent>(TEXT("Jelly Effect Component"));
	JellyEffectComponent->SetActive(true);
	//JellyEffectComponent->SetMeshComponent(GetMesh());
	//JellyEffectComponent->SetAutoActivate(true);

	//NicknameWidgetComponent = CreateDefaultSubobject <UWidgetComponent>(TEXT("Nickname Widget Component"));
	//NicknameWidgetComponent->SetupAttachment(RootComponent);

	HealthBarWidgetComponent = CreateDefaultSubobject <UWidgetComponent>(TEXT("Health Bar Widget Component"));
	HealthBarWidgetComponent->SetupAttachment(RootComponent);
	HealthBarWidgetComponent->SetComponentTickInterval(1.0f);

	
	AttackComponent = CreateDefaultSubobject <UUSACharacterAttackComponent>(TEXT("Attack Component"));


	//TestStaticMeshComponent = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Test Static Mesh"));
	//TestStaticMeshComponent->SetupAttachment(RootComponent);

	//PivotComponent = CreateDefaultSubobject <UUSACharacterPivotComponent>(TEXT("Character Pivot Component"));
	//PivotComponent->SetupAttachment(RootComponent);

	//CharacterCapsuleWalkInfo.RenewCharacterCapsule(this);
	//CharacterMovementWalkInfo.RenewCharacterMovementInfo(GetCharacterMovement());

	//GetCharacterMovement()->MovementState.bCanCrouch = true;
	//GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeedCrouched = 0.0f;
	GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;

	//WeaponDetectBoxComponent = CreateDefaultSubobject <UBoxComponent>(TEXT("Weapon Detect Box Component"));
	//WeaponDetectBoxComponent->SetupAttachment(RootComponent);
	//WeaponDetectBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AUSACharacterBase::OnWeaponDetectBoxOverlapBegin);
	//WeaponDetectBoxComponent->SetGenerateOverlapEvents(false);

	ASC = nullptr;

	CurrentEquipedWeapons.Init(nullptr, 10);
	StartWeaponClassList.Init(nullptr, 10);

	//bIsSetStartWeaponBeforeGASSetup = false;

	//NetUpdateFrequency = 200.0f;
}

void AUSACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//if (IsValid(GetCapsuleComponent()) == true)
	//{
	//	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	//}

	UUSAGameInstance* USAGameInstance = Cast<UUSAGameInstance>(GetGameInstance());
	if (IsValid(USAGameInstance) == true)
	{
		USADamageType_Explosion = USAGameInstance->GetUSADamageType_Explosion();
		USADamageType_Grab = USAGameInstance->GetUSADamageType_Grab();
		
		USAItemOrder = USAGameInstance->GetUSAItemOrder();
		CurrentUSAItemOrderIndex = 0;
	}
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

	//

	CharacterCapsuleInfos[KEYNAME_CAPSULEINFO_WALK].RenewCharacterCapsule(this, CameraSpringArmComponent);

	//

	if (IsValid(GetMesh()->GetAnimInstance()) == true)
	{
		GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &AUSACharacterBase::USACharacterAnimInstanceMontageNotify);
	}

	//

	//WeaponDetectBoxComponent->SetGenerateOverlapEvents(false);

	//if (GetWorld()->GetTimerManager().IsTimerActive(WeaponDetectBoxComponentTimerHandle))
	//{
	//	GetWorld()->GetTimerManager().ClearTimer(WeaponDetectBoxComponentTimerHandle);
	//	WeaponDetectBoxComponentTimerHandle.Invalidate();
	//}

	//GetWorld()->GetTimerManager().SetTimer(WeaponDetectBoxComponentTimerHandle, FTimerDelegate::CreateLambda([=]()
	//	{
	//		if (IsValid(WeaponDetectBoxComponent) == true)
	//		{
	//			WeaponDetectBoxComponent->SetGenerateOverlapEvents(true);
	//		}
	//	}
	//), 10.0f, false);

	//

	if (JellyEffectComponent != nullptr)
	{
		JellyEffectComponent->SetJellySceneComponent(this, GetMesh());
	}

	//

	K2_OnCurrentHealthRatioChanged
	(
		GetCharacterCurrentHealthRatio_Implementation(), 
		GetCharacterMaxHealth_Implementation(), 
		GetCharacterCurrentHealth_Implementation()
	);

	// 

	K2_OnCharacterNameChanged();

	//USA_LOG(LogTemp, Log, TEXT("Hey"));
}

// Called every frame
void AUSACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUSACharacterBase::BeginDestroy()
{
	Super::BeginDestroy();
}

void AUSACharacterBase::Destroyed()
{
	if (ASC != nullptr)
	{
		ASC->ClearAllAbilities();
	}

	// 무기 드랍
	DropWeapons(true);

	Super::Destroyed();
}


//void AUSACharacterBase::OnConstruction(const FTransform& Transform)
//{
//	Super::OnConstruction(Transform);
//
//	//CharacterCapsuleWalkInfo.RenewCharacterCapsule(this);
//	CharacterMovementWalkInfo.RenewCharacterMovementInfo(GetCharacterMovement());
//}

bool AUSACharacterBase::ServerRPC_RenewCharacterCapsule_Validate(/*ACharacter* InCharacter, */const FName& InKeyName)
{
	return true;
}

void AUSACharacterBase::ServerRPC_RenewCharacterCapsule_Implementation(/*ACharacter* InCharacter, */const FName& InKeyName)
{
	MulticastRPC_RenewCharacterCapsule(/*InCharacter, */InKeyName);
}

void AUSACharacterBase::MulticastRPC_RenewCharacterCapsule_Implementation(/*ACharacter* InCharacter, */const FName& InKeyName)
{
	//if (InCharacter == nullptr)
	//{
	//	return;
	//}

	if (CharacterCapsuleInfos.Num() <= 0)
	{
		return;
	}

	if (CharacterCapsuleInfos.Contains(InKeyName) == false)
	{
		return;
	}

	CharacterCapsuleInfos[InKeyName].RenewCharacterCapsule(this, CameraSpringArmComponent);
	CharacterCapsuleInfos[InKeyName].RenewJellyEffectMeshLocation(JellyEffectComponent);
	//TestStaticMeshComponent->SetRelativeLocation(CharacterCapsuleInfos[InKeyName].CapsuleHaflHeight * FVector::RightVector);
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

void AUSACharacterBase::StopAndPlayAnimMontageByGroup(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	ServerRPC_StopAndPlayAnimMontageByGroup(AnimMontage, InPlayRate, StartSectionName);
}

void AUSACharacterBase::ServerRPC_StopAndPlayAnimMontageByGroup_Implementation(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	MulticastRPC_StopAndPlayAnimMontageByGroup(AnimMontage, InPlayRate, StartSectionName);
}

void AUSACharacterBase::MulticastRPC_StopAndPlayAnimMontageByGroup_Implementation(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	UAnimInstance* AnimInstance = (GetMesh()) ? GetMesh()->GetAnimInstance() : nullptr;

	if (IsValid(AnimMontage) == true
		&& IsValid(AnimInstance) == true
		/*&& IsValid(InASC) == true
		&& InASC->GetGameplayTagCount(USA_CHARACTER_ACTION) <= 0*/)
	{
		AnimInstance->Montage_StopGroupByName(0.0f, AnimMontage->GetGroupName());
		AnimInstance->Montage_Play(AnimMontage, InPlayRate, EMontagePlayReturnType::MontageLength, 0.0f, false);
	}
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

//void AUSACharacterBase::AddNextWaitingWeapon(AUSAWeaponBase* InWeapon)
//{
//	//MulticastRPC_SetNextWeapon(InNextWeapon);
//	
//	if (UKismetSystemLibrary::IsStandalone(GetWorld())
//		|| UKismetSystemLibrary::IsServer(GetWorld()))
//	{
//
//	}
//	else
//	{
//		return;
//	}
//
//	//if (HasAuthority() == false)
//	//{
//	//	return;
//	//}
//	
//	if (InWeapon != nullptr)
//	{
//		NextWaitingWeapons.Add(InWeapon);
//
//		OnRep_NextWaitingWeapon();
//	}
//}

//void AUSACharacterBase::MulticastRPC_SetNextWeapon_Implementation(AUSAWeaponBase* InNextWeapon)
//{
//	//if (HasAuthority())
//	//{
//	//	
//	//}
//}

//void AUSACharacterBase::OnRep_NextWaitingWeapon()
//{
//	if (ASC == nullptr)
//	{
//		bIsSetStartWeaponBeforeGASSetup = true;
//
//		return;
//	}
//
//	//if (HasAuthority() == true)
//	//{
//	//	UpdateCurrentWeapons();
//	//}
//}


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
	//bool bIsInstantTargeting = false;

	// 우선 널 포인터라면 임시 타겟팅으로 판단
	//if (CurrentTargetableActor == nullptr)
	//{
	//	bIsInstantTargeting = true;
	//	UpdateCurrentTargetableActor_Instant();
	//}
	
	if (IsValid(CurrentTargetableActor) == false && IsValid(CurrentTargetableActor_Instant) == false)
	{
		return GetUSACharacterDirection_InputMovement();
	}

	if (Cast<AActor>(CurrentTargetableActor) == nullptr && Cast<AActor>(CurrentTargetableActor_Instant) == nullptr)
	{
		return GetUSACharacterDirection_InputMovement();
	}
	
	FVector Result = FVector::ForwardVector;

	if (IsValid(CurrentTargetableActor) == true)
	{
		FVector SourceLocation = GetActorLocation();
		FVector TargetLocation = Cast<AActor>(CurrentTargetableActor)->GetActorLocation();

		Result = TargetLocation - SourceLocation;
	}
	else if (IsValid(CurrentTargetableActor_Instant) == true)
	{
		FVector SourceLocation = GetActorLocation();
		FVector TargetLocation = Cast<AActor>(CurrentTargetableActor_Instant)->GetActorLocation();

		Result = TargetLocation - SourceLocation;
	}

	Result.Z = 0.0f;

	Result.Normalize();

	// 임시 타겟팅인 경우, 원상복구
	//if (bIsInstantTargeting)
	//{
	//	CurrentTargetableActor = nullptr;
	//}

	return Result;
}

void AUSACharacterBase::StartCameraShake_HitSuccess(TSubclassOf<class UDamageType> DamageType)
{
	// ...
}

void AUSACharacterBase::LookTarget(const struct FInputActionValue& Value)
{
	// ...
}


//

void AUSACharacterBase::OnPickableDetectBoxOverlapBegin
(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ...
}

//

void AUSACharacterBase::Falling()
{
	Super::Falling();

	ServerRPC_RenewCharacterCapsule(/*this, */KEYNAME_CAPSULEINFO_FALL);
}

void AUSACharacterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	ServerRPC_RenewCharacterCapsule(/*this, */KEYNAME_CAPSULEINFO_WALK);
}

void AUSACharacterBase::SetPlayerDefaults()
{
	ResetAttributeSet();

	RespawnUSACharacter();
}

//

void AUSACharacterBase::OnUSACrouch()
{
	ServerRPC_OnUSACrouch();
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
	ServerRPC_OnUSAUnCrouch();
}

void AUSACharacterBase::DownUSACharacter()
{
	K2_OnUSADeath();
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


// Called to bind functionality to input
void AUSACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUSACharacterBase::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AUSACharacterBase::MoveEnd);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUSACharacterBase::Look);
		EnhancedInputComponent->BindAction(TargetAction, ETriggerEvent::Triggered, this, &AUSACharacterBase::LookTarget);

		//EnhancedInputComponent->BindAction(PrevItemAction, ETriggerEvent::Triggered, this, &AUSACharacterBase::PrevItem);
		//EnhancedInputComponent->BindAction(NextItemAction, ETriggerEvent::Triggered, this, &AUSACharacterBase::NextItem);

		//EnhancedInputComponent->BindAction(ItemAction, ETriggerEvent::Triggered, this, &AUSACharacterBase::UseItem);
		//EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Triggered, this, &AUSACharacterBase::DoDrop);

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

		//TArray<UInputAction*> InputKeys;
		//GameplayTagInputInfos.GenerateKeyArray(InputKeys);

		//for (UInputAction* Key : InputKeys)
		//{
		//		EnhancedInputComponent->BindAction(Key, ETriggerEvent::Triggered,
		//			this, &AUSACharacterBase::ActiveGameplayTagInput_Pressed, Key);
		//		EnhancedInputComponent->BindAction(Key, ETriggerEvent::Completed,
		//			this, &AUSACharacterBase::ActiveGameplayTagInput_Released, Key);
		//}
	}
}

void AUSACharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 서버에서 수행

	if (bIsASCInitialized == false)
	{
		SetupGAS();

		PostSetupGAS();

		SetupAttributeSet();

		ResetAttributeSet();

		BeginStartAbilities();
	}

	// 시작할 때 자동으로 콘솔 입력
	//APlayerController* PlayerController = Cast <APlayerController>(NewController);
	//if (PlayerController != nullptr)
	//{
	//	PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
	//}

	//USA_LOG(LogTemp, Log, TEXT("Hey"));
}

void AUSACharacterBase::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	if (ASC != nullptr
		&& ASC->GetGameplayTagCount(USA_CHARACTER_ADJUST_IGNOREMOVEINPUT) > 0)
	{
		return;
	}

	Super::AddMovementInput(WorldDirection, ScaleValue, bForce);
}

void AUSACharacterBase::USACharacterAnimInstanceMontageNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == TEXT("AllWeapon_Holder"))
	{
		AttachWeaponToHolderSocket(CurrentEquipedWeapons[0]);

		AttachWeaponToHolderSocket(CurrentEquipedWeapons[1]);
	}

	//

	if (NotifyName == TEXT("FirstWeapon_Holder"))
	{
		AttachWeaponToHolderSocket(CurrentEquipedWeapons[0]);
	}

	if (NotifyName == TEXT("FirstWeapon_Hand"))
	{
		AttachWeaponToHandSocket(CurrentEquipedWeapons[0]);
	}

	if (NotifyName == TEXT("SecondWeapon_Holder"))
	{
		AttachWeaponToHolderSocket(CurrentEquipedWeapons[1]);
	}

	if (NotifyName == TEXT("SecondWeapon_Hand"))
	{
		AttachWeaponToHandSocket(CurrentEquipedWeapons[1]);
	}
}

void AUSACharacterBase::ChangeItem(bool InIsChangeToNext)
{
	if (USAItemOrder.Num() == 0)
	{
		K2_OnCurrentItemOrderIndexChanged(nullptr);
		return;
	}

	if (InIsChangeToNext)
	{
		CurrentUSAItemOrderIndex += 1;
		CurrentUSAItemOrderIndex %= USAItemOrder.Num();

	}
	else
	{
		CurrentUSAItemOrderIndex += USAItemOrder.Num() - 1;
		CurrentUSAItemOrderIndex %= USAItemOrder.Num();

	}
	
	K2_OnCurrentItemOrderIndexChanged(GetCurrentItemClass());
	K2_OnCurrentItemCountChanged(GetCurrentItemCount());
}

TSubclassOf<class AUSAItemBase> AUSACharacterBase::GetCurrentItemClass()
{
	if (USAItemOrder.Num() == 0)
	{
		return nullptr;
	}

	if (USAItemOrder.IsValidIndex(CurrentUSAItemOrderIndex) == false)
	{
		return nullptr;
	}

	return USAItemOrder[CurrentUSAItemOrderIndex];
}

int32 AUSACharacterBase::GetCurrentItemCount()
{
	int32 ItemCount = 0;
	
	if (USAItemOrder.IsValidIndex(CurrentUSAItemOrderIndex))
	{
		TSubclassOf<AUSAItemBase> CurrentItemClass = USAItemOrder[CurrentUSAItemOrderIndex];

		for (TSubclassOf<AUSAItemBase> Item : CurrentOwnedItems)
		{
			if (Item == CurrentItemClass)
			{
				ItemCount += 1;
			}
		}
	}

	return ItemCount;
}

void AUSACharacterBase::UpdateMaterialParameter(int InIndex, FName InName, float InValue)
{
	if (IsValid(GetMesh()) == false)
	{
		return;
	}

	if (MaterialInstanceDynamicList.Num() == 0)
	{
		MaterialInstanceDynamicList.Init(nullptr, GetMesh()->GetMaterials().Num());
	}

	if (MaterialInstanceDynamicList.IsValidIndex(InIndex) == false)
	{
		return;
	}

	if (MaterialInstanceDynamicList[InIndex] == nullptr)
	{
		MaterialInstanceDynamicList[InIndex] = GetMesh()->CreateDynamicMaterialInstance(InIndex);
	}

	if (IsValid(MaterialInstanceDynamicList[InIndex]) == false)
	{
		return;
	}

	MaterialInstanceDynamicList[InIndex]->SetScalarParameterValue(InName, InValue);
}

//

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
	//if (bIsCurrentInputPressedIDMap.Contains(InputID) == false)
	//{
	//	bIsCurrentInputPressedIDMap.Add({ InputID , false });
	//}

	//if (bIsCurrentInputPressedIDMap[InputID] == true)
	//{
	//	return;
	//}

	if (ASC == nullptr)
	{
		return;
	}

	//USA_LOG(LogTemp, Log, TEXT("Input Pressed Gameplay Ability Activate"));

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

	//bIsCurrentInputPressedIDMap[InputID] = true;
}

void AUSACharacterBase::InputReleaseGameplayAbilityByInputID(int32 InputID)
{
	//if (bIsCurrentInputPressedIDMap.Contains(InputID) == false)
	//{
	//	bIsCurrentInputPressedIDMap.Add({ InputID , true });
	//}

	//if (bIsCurrentInputPressedIDMap[InputID] == false)
	//{
	//	return;
	//}

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

	//bIsCurrentInputPressedIDMap[InputID] = false;
}
//
//void AUSACharacterBase::ActiveGameplayTagInput_Pressed(UInputAction* InInput)
//{
//	if (bIsCurrentInputPressedMap.Contains(InInput) == false)
//	{
//		bIsCurrentInputPressedMap.Add({ InInput, false });
//	}
//
//	if (bIsCurrentInputPressedMap[InInput] == true)
//	{
//		return;
//	}
//	
//	if (ASC == nullptr)
//	{
//		return;
//	}
//
//	if (GameplayTagInputInfos.Contains(InInput) == false)
//	{
//		return;
//	}
//
//	USA_LOG(LogTemp, Log, TEXT("Input Pressed "));
//
//	//ASC->AddLooseGameplayTag(GameplayTagInputInfos[InInput].InputGameplayTag_Holding);
//	ServerRPC_AddRemovedGameplayTag(GameplayTagInputInfos[InInput].InputGameplayTag_Pressed, true);
//	ServerRPC_AddRemovedGameplayTag(GameplayTagInputInfos[InInput].InputGameplayTag_Holding, true);
//	ServerRPC_AddRemovedGameplayTag(GameplayTagInputInfos[InInput].InputGameplayTag_Released, false);
//
//	if (CurrentInputTimerHandleMap.Contains(InInput) == false)
//	{
//		CurrentInputTimerHandleMap.Add({ InInput , FTimerHandle()});
//	}
//	else
//	{
//		CurrentInputTimerHandleMap[InInput].Invalidate();
//	}
//
//	GetWorld()->GetTimerManager().SetTimer(CurrentInputTimerHandleMap[InInput], FTimerDelegate::CreateLambda([=]()
//		{
//			ServerRPC_AddRemovedGameplayTag(GameplayTagInputInfos[InInput].InputGameplayTag_Pressed, false);
//			ServerRPC_AddRemovedGameplayTag(GameplayTagInputInfos[InInput].InputGameplayTag_Released, false);
//		}
//	), CurrentInputMaintainTime, false);
//
//	bIsCurrentInputPressedMap[InInput] = true;
//}
//
//void AUSACharacterBase::ActiveGameplayTagInput_Released(UInputAction* InInput)
//{
//	if (bIsCurrentInputPressedMap.Contains(InInput) == false)
//	{
//		bIsCurrentInputPressedMap.Add({ InInput, true });
//	}
//
//	if (bIsCurrentInputPressedMap[InInput] == false)
//	{
//		return;
//	}
//
//	if (ASC == nullptr)
//	{
//		return;
//	}
//
//	if (GameplayTagInputInfos.Contains(InInput) == false)
//	{
//		return;
//	}
//
//	USA_LOG(LogTemp, Log, TEXT("Input Released"));
//
//	//ASC->RemoveLooseGameplayTag(GameplayTagInputInfos[InInput].InputGameplayTag_Holding);
//	ServerRPC_AddRemovedGameplayTag(GameplayTagInputInfos[InInput].InputGameplayTag_Pressed, false);
//	ServerRPC_AddRemovedGameplayTag(GameplayTagInputInfos[InInput].InputGameplayTag_Holding, false);
//	ServerRPC_AddRemovedGameplayTag(GameplayTagInputInfos[InInput].InputGameplayTag_Released, true);
//
//	if (CurrentInputTimerHandleMap.Contains(InInput) == false)
//	{
//		CurrentInputTimerHandleMap.Add({ InInput , FTimerHandle() });
//	}
//	else
//	{
//		CurrentInputTimerHandleMap[InInput].Invalidate();
//	}
//
//	GetWorld()->GetTimerManager().SetTimer(CurrentInputTimerHandleMap[InInput], FTimerDelegate::CreateLambda([=]()
//		{
//			ServerRPC_AddRemovedGameplayTag(GameplayTagInputInfos[InInput].InputGameplayTag_Pressed, false);
//			ServerRPC_AddRemovedGameplayTag(GameplayTagInputInfos[InInput].InputGameplayTag_Released, false);
//		}
//	), CurrentInputMaintainTime, false);
//
//	bIsCurrentInputPressedMap[InInput] = false;
//}

//void AUSACharacterBase::ServerRPC_AddRemovedGameplayTag_Implementation(const FGameplayTag InTag, bool InAdded)
//{
//	MulticastRPC_AddRemovedGameplayTag(InTag, InAdded);
//}
//
//void AUSACharacterBase::MulticastRPC_AddRemovedGameplayTag_Implementation(const FGameplayTag InTag, bool InAdded)
//{
//	if (ASC == nullptr)
//	{
//		return;
//	}
//
//	if (InAdded)
//	{
//		ASC->AddLooseGameplayTag(InTag);
//
//		TryGameplayAbilityByGameplayTag(InTag.GetTagName());
//	}
//	else
//	{
//		ASC->RemoveLooseGameplayTag(InTag);
//	}
//}


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

void AUSACharacterBase::OnGameplayTagCallback_OffOverlapEvent(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		bIsPickable = false;
	}
	else
	{
		bIsPickable = true;
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
		
	}
	else
	{
		
	}
}

void AUSACharacterBase::OnGameplayTagCallback_Fall(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		if (IsValid(GetCharacterMovement()))
		{
			//GetCharacterMovement()->bUseFlatBaseForFloorChecks = false;
		}
	}
	else
	{
		if (IsValid(GetCharacterMovement()))
		{
			//GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;
		}
	}
}

void AUSACharacterBase::OnGameplayTagCallback_Slide(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		
	}
	else
	{
		
	}
}

void AUSACharacterBase::OnGameplayTagCallback_Crouch(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		OnUSACrouch();
		ServerRPC_RenewCharacterCapsule(/*this, */KEYNAME_CAPSULEINFO_CROUCH);
	}
	else
	{
		OnUSAUnCrouch();

		if (GetMovementComponent()->IsFalling())
		{
			ServerRPC_RenewCharacterCapsule(/*this, */KEYNAME_CAPSULEINFO_FALL);
		}
		else
		{
			ServerRPC_RenewCharacterCapsule(/*this, */KEYNAME_CAPSULEINFO_WALK);
		}
	}
}


void AUSACharacterBase::OnGameplayTagCallback_Dead(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

		DropWeapons(true);

		DownUSACharacter();
	}
	else
	{
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	}
}

void AUSACharacterBase::OnGameplayTagCallback_Action(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{

	}
	else
	{

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
	(EAttachmentRule::SnapToTarget, true);

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
	(EAttachmentRule::SnapToTarget, true);

	InWeapon->AttachToComponent(GetMesh(), AttachmentTransformRules, InWeapon->GetWeaponHolderSocketName());
}

void AUSACharacterBase::AttachAllWeaponToHolderSocket()
{
	for (AUSAWeaponBase* Weapon : CurrentEquipedWeapons)
	{
		AttachWeaponToHolderSocket(Weapon);
	}
}

bool AUSACharacterBase::SetCurrentWeapon(int32 InEquipIndex, AUSAWeaponBase* InWeapon)
{
	if (UKismetSystemLibrary::IsServer(GetWorld()) == false
		&& UKismetSystemLibrary::IsStandalone(GetWorld()) == false)
	{
		return false;
	}

	if (CurrentEquipedWeapons.IsValidIndex (InEquipIndex) == false)
	{
		return false;
	}

	CurrentEquipedWeapons[InEquipIndex] = InWeapon;

	return true;
}

void AUSACharacterBase::PickUpSomething(IUSAPickableInterface* InPick)
{
	// 서버에서만 수행하도록
	if (UKismetSystemLibrary::IsServer(GetWorld()) == false
		&& UKismetSystemLibrary::IsStandalone(GetWorld()) == false)
	{
		return;
	}

	if (InPick == nullptr)
	{
		return;
	}

	TArray<AUSAWeaponBase*> PrevWeapons = CurrentEquipedWeapons;
	TArray<TSubclassOf<class AUSAItemBase>> PrevItems = CurrentOwnedItems;

	AUSAWeaponBase* WeaponBase = Cast<AUSAWeaponBase>(InPick);
	AUSAItemBase* ItemBase = Cast<AUSAItemBase>(InPick);

	if (WeaponBase)
	{
		if (InPick->PickUpByUSACharacter(ASC, this))
		{
			OnRep_CurrentEquipedWeapons(PrevWeapons);
		}
	}

	if (ItemBase)
	{
		if (InPick->PickUpByUSACharacter(ASC, this))
		{
			OnRep_CurrentOwnedItems(PrevItems);
		}
	}
}


bool AUSACharacterBase::GetIsThereAnyWeapon()
{
	for (AUSAWeaponBase* Weapon : CurrentEquipedWeapons)
	{
		if (IsValid(Weapon) == true)
		{
			return true;
		}
	}

	return false;
}

void AUSACharacterBase::DropWeapons(bool bIsAbsolute)
{
	if (bIsAbsolute)
	{
		ServerRPC_DropWeapons();
	}
	else
	{
		if (IsValid(ASC) == true
			&& ASC->GetGameplayTagCount(USA_CHARACTER_ACTION) <= 0)
		{
			ServerRPC_DropWeapons();
		}
	}
}	

//

void AUSACharacterBase::RespawnUSACharacter()
{
	ServerRPC_RespawnUSACharacter();
}


void AUSACharacterBase::ServerRPC_RespawnUSACharacter_Implementation()
{
	MulticastRPC_RespawnUSACharacter();
}

void AUSACharacterBase::MulticastRPC_RespawnUSACharacter_Implementation()
{
	ResetAttributeSet();

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

		// 리스폰
		FGameplayAbilitySpec* GameplayAbilitySpec_Respawn = ASC->FindAbilitySpecFromClass(GameplayAbility_Respawn);

		if (GameplayAbilitySpec_Respawn)
		{
			if (GameplayAbilitySpec_Respawn->IsActive())
			{
				ASC->AbilitySpecInputPressed(*GameplayAbilitySpec_Respawn);
			}
			else
			{
				ASC->TryActivateAbility(GameplayAbilitySpec_Respawn->Handle);
			}
		}
	}

	K2_OnUSARespawn();
}

//

void AUSACharacterBase::WinUSACharacter()
{
	ServerRPC_WinUSACharacter();
}

void AUSACharacterBase::ServerRPC_WinUSACharacter_Implementation()
{
	MulticastRPC_WinUSACharacter();
}

void AUSACharacterBase::MulticastRPC_WinUSACharacter_Implementation()
{
	if (ASC != nullptr)
	{
		// 승리
		FGameplayAbilitySpec* GameplayAbilitySpec_Win = ASC->FindAbilitySpecFromClass(GameplayAbility_Win);

		if (GameplayAbilitySpec_Win)
		{
			if (GameplayAbilitySpec_Win->IsActive())
			{
				ASC->AbilitySpecInputPressed(*GameplayAbilitySpec_Win);
			}
			else
			{
				ASC->TryActivateAbility(GameplayAbilitySpec_Win->Handle);
			}
		}
	}

	K2_OnUSAWin();
}

//

void AUSACharacterBase::SetCurrentWeaponsUsingStartWeaponClassList()
{
	ServerRPC_SetCurrentWeaponsUsingStartWeaponClassList();
}

void AUSACharacterBase::ServerRPC_SetCurrentWeaponsUsingStartWeaponClassList_Implementation()
{
	for (TSubclassOf <AUSAWeaponBase> StartWeaponClass : StartWeaponClassList)
	{
		if (IsValid(StartWeaponClass) == false)
		{
			continue;
		}

		AUSAWeaponBase* StartWeapon = GetWorld()->SpawnActor<AUSAWeaponBase>(StartWeaponClass, GetActorTransform());
		//PickupWeapon(StartWeapon);

		IUSAPickableInterface* StartWeaponInterface = Cast<IUSAPickableInterface>(StartWeapon);
		PickUpSomething(StartWeaponInterface);
	}
}


void AUSACharacterBase::ServerRPC_DropWeapons_Implementation()
{
	// 서버 클라 모두 수행
	MulticastRPC_DropWeapons();
}

void AUSACharacterBase::MulticastRPC_DropWeapons_Implementation()
{
	for (AUSAWeaponBase* CurrentWeapon: CurrentEquipedWeapons)
	{
		if (IsValid(CurrentWeapon) == false)
		{
			continue;
		}

		CurrentWeapon->SetPickableActorOwner(nullptr);
	}
}

bool AUSACharacterBase::GetIsTargetableCurrently()
{
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
	//if (GetMesh() != nullptr 
	//	&& GetMesh()->DoesSocketExist(TargetablePivotName) == true)
	//{
	//	FVector ResultLocation = GetMesh()->GetSocketLocation(TargetablePivotName);
	//	DrawDebugSphere(GetWorld(), ResultLocation, 15.0f, 8, FColor::Red, false, -1.0f, 0U, 2.0f);
	//}

	return GetActorLocation();
}

FVector AUSACharacterBase::GetTargetableToplocation()
{
	//if (GetMesh() != nullptr
	//	&& GetMesh()->DoesSocketExist(TargetablePivotName) == true)
	//{
	//	FVector ResultLocation = GetMesh()->GetSocketLocation(TargetablePivotName);
	//	DrawDebugSphere(GetWorld(), ResultLocation, 15.0f, 8, FColor::Red, false, -1.0f, 0U, 2.0f);
	//}

	if (GetCapsuleComponent())
	{
		return GetActorLocation() + FVector::UpVector * GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	}

	return GetActorLocation();
}

float AUSACharacterBase::GetTargetableCapsuleRadius()
{
	if (IsValid(GetCapsuleComponent()) == true)
	{
		return GetCapsuleComponent()->GetScaledCapsuleRadius();
	}

	return 50.0f;
}

bool AUSACharacterBase::PreUseItem()
{
	if (USAItemOrder.IsValidIndex(CurrentUSAItemOrderIndex) == false)
	{
		return false;
	}

	TSubclassOf<AUSAItemBase> CurrentItemClass = USAItemOrder[CurrentUSAItemOrderIndex];

	if (IsValid(CurrentItemClass) == false)
	{
		return false;
	}

	if (CurrentOwnedItems.Contains(CurrentItemClass) == false)
	{
		return false;
	}

	Cast<AUSAItemBase>(CurrentItemClass->GetDefaultObject())->PlayUseAnimationMontageInUSACharacter(ASC, this);

	return true;
}

bool AUSACharacterBase::PostUseItem()
{
	if (CurrentUSAItemOrderIndex < 0 || USAItemOrder.Num() <= CurrentUSAItemOrderIndex)
	{
		return false;
	}

	TSubclassOf<AUSAItemBase> CurrentItemClass = USAItemOrder[CurrentUSAItemOrderIndex];

	if (IsValid(CurrentItemClass) == false)
	{
		return false;
	}

	if (CurrentOwnedItems.Contains(CurrentItemClass) == false)
	{
		return false;
	}

	Cast<AUSAItemBase>(CurrentItemClass->GetDefaultObject())->ActiveUSAItem(ASC, this);

	TArray<TSubclassOf<AUSAItemBase>> PrevItems = CurrentOwnedItems;

	if (UKismetSystemLibrary::IsServer(GetWorld()) == true
		|| UKismetSystemLibrary::IsStandalone(GetWorld()) == true)
	{
		int ItemIndex = 0;
		CurrentOwnedItems.Find(CurrentItemClass, ItemIndex);
		CurrentOwnedItems.RemoveAt(ItemIndex);

		OnRep_CurrentOwnedItems(PrevItems);
	}

	return true;
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
	// 서버에서만 수행
	if (GetWorld()->GetAuthGameMode() == nullptr)
	{
		return 0;
	}

	float ResultDamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//USA_LOG(LogTemp, Log, TEXT("Taking Damage..."));

	// 데미지
	if (ASC && ASC->GetGameplayTagCount(USA_CHARACTER_STATE_INVINCIBLE) > 0)
	{
		return 0;
	}

	// 팀킬 방지
	// 서버에서만 수행하기 때문에 컨트롤러를 활용할 수 있음
	AAIController* EventAIController = Cast<AAIController>(EventInstigator);
	APlayerController* EventPlayerController = Cast<APlayerController>(EventInstigator);

	AAIController* MyAIController = Cast<AAIController>(GetController());
	APlayerController* MyPlayerController = Cast<APlayerController>(GetController());

	// 같은 팀 (서로 같은 컨트롤러를 검사하는 이유는 스스로 맞는 공격을 구현하기 위함)
	if (MyAIController && EventAIController && (MyAIController != EventAIController))
	{
		return 0;
	}

	if (MyPlayerController && EventPlayerController && (MyPlayerController != EventPlayerController))
	{
		return 0;
	}
	
	// 패리 중일 때는 데미지 무시
	if (IsValid(ASC) == true
		&& ASC->HasMatchingGameplayTag(USA_CHARACTER_ACTION_PARRY) == false)
	{
		UUSAAttributeSet* USAAttributeSet = const_cast<UUSAAttributeSet*>(ASC->GetSet<UUSAAttributeSet>());

		if (USAAttributeSet != nullptr)
		{
			USAAttributeSet->SetDamage(DamageAmount);
		}

		MulticastRPC_TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

		//if (DamageCauser && EventInstigator)
		//{
		//	if (EventInstigator->IsLocalController())
		//	{
		//		if (AUSACharacterBase* CharacterCauser = Cast<AUSACharacterBase>(DamageCauser))
		//		{
		//			CharacterCauser->StartCameraShake_HitSuccess(DamageEvent.DamageTypeClass);
		//		}
		//	}
		//	else
		//	{
		//		MulticastRPC_TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
		//	}
		//}
	}

	// 넉백 어빌리티 수행
	APawn* EventInstigatorPawn = nullptr;
	if (IsValid(EventInstigator) == true)
	{
		EventInstigatorPawn = EventInstigator->GetPawn();
	}

	ApplyDamageMomentum(ResultDamageAmount, DamageEvent, EventInstigatorPawn, DamageCauser);

	ApplyDamageUSAJellyEffect(DamageEvent.DamageTypeClass);

	return ResultDamageAmount;
}

void AUSACharacterBase::ClientRPC_TakeDamage_Implementation(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 가해자의 카메라 쉐이크 수행 
	if (AUSACharacterBase* CharacterCauser = Cast<AUSACharacterBase>(DamageCauser))
	{
		CharacterCauser->StartCameraShake_HitSuccess(DamageEvent.DamageTypeClass);
	}
}

void AUSACharacterBase::MulticastRPC_TakeDamage_Implementation(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//if (ASC == nullptr)
	//{
	//	return;
	//}

	//// 패리 중일 때는 데미지 무시
	//if (ASC->HasMatchingGameplayTag(USA_CHARACTER_ACTION_PARRY))
	//{
	//	return;
	//}

	// 데미지 적용

	// 
	//UUSAAttributeSet* USAAttributeSet = const_cast<UUSAAttributeSet*>(ASC->GetSet<UUSAAttributeSet>());

	//if (USAAttributeSet != nullptr)
	//{
	//	USAAttributeSet->SetDamage(DamageAmount);
	//}

	// 가해자의 카메라 쉐이크 수행 
	if (AUSACharacterBase* CharacterCauser = Cast<AUSACharacterBase>(DamageCauser))
	{
		CharacterCauser->StartCameraShake_HitSuccess(DamageEvent.DamageTypeClass);
	}
}


void AUSACharacterBase::ApplyDamageMomentum(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
{
	if (ASC == nullptr)
	{
		return;
	}

	// 죽은 상태인 경우 넉백 무시
	if (ASC->HasMatchingGameplayTag(USA_CHARACTER_STATE_DEAD))
	{
		return;
	}

	// bIsJustDead 
	// 데미지 적용 후 죽었는지 판단하기 위한 변수
	float CheckCurrentHealth = 0.0f;
	bool CheckIsAttributeFound = false;
	CheckCurrentHealth = ASC->GetGameplayAttributeValue(UUSAAttributeSet::GetCurrentHealthAttribute(), CheckIsAttributeFound);

	bool bIsJustDead = false;
	if (CheckIsAttributeFound == true
		&& CheckCurrentHealth <= 0.0f)
	{
		bIsJustDead = true;
	}
	
	// bIsParrying
	// 현재 팩링 중인지 판단하기 위한 변수

	bool bIsParrying = false;
	if (ASC->HasMatchingGameplayTag(USA_CHARACTER_ACTION_PARRY))
	{
		bIsParrying = true;
	}

	// bIsFalling
	// 현재 공중에 있는지 판단하기 위한 변수
	bool bIsFallingLocally = GetMovementComponent()->IsFalling();

	// bIsBiggerDamageThanArmor
	// 현재 들어온 데미지가 아머보다 높은지 판단하기 위한 변수
	bool bIsBiggerDamageThanArmor = true;

	float CurrentArmor = 0.0f;
	bool CheckIsCurrentArmorFound = false;
	CurrentArmor = ASC->GetGameplayAttributeValue(UUSAAttributeSet::GetCurrentArmorAttribute(), CheckIsCurrentArmorFound);
	if (DamageTaken < CurrentArmor)
	{
		bIsBiggerDamageThanArmor = false;
	}

	FVector NewDirection = FVector::ForwardVector;
	TSubclassOf<UGameplayAbility> DamageAbilityClass;

	FVector AttackDirection;
	FHitResult HitResult;
	TSubclassOf<UDamageType> DamageType;

	DamageEvent.GetBestHitInfo(nullptr, nullptr, HitResult, AttackDirection);
	DamageType = DamageEvent.DamageTypeClass;

	// 공격 타입 판단

	// 폭파
	if (IsValid(USADamageType_Explosion) == true
		&& USADamageType_Explosion == DamageType)
	{
		NewDirection = ((HitResult.TraceStart + HitResult.TraceEnd) * 0.5f) - GetActorLocation();
		NewDirection.Z = 0.0f;
		NewDirection.Normalize();
	}
	// 그랩
	else if (IsValid(USADamageType_Grab) == true
		&& USADamageType_Grab == DamageType)
	{
		// 커스텀 로케이션
		ActionCustomLocation = HitResult.TraceStart;

		NewDirection = AttackDirection * -1.0f;
		NewDirection.Z = 0.0f;
		NewDirection.Normalize();
	}
	// 그 외
	else
	{
		NewDirection = AttackDirection * -1.0f;
		NewDirection.Z = 0.0f;
		NewDirection.Normalize();
	}

	//SetActorRotation(NewDirection.Rotation());
	//UpdateComponentTransforms();

	// 죽음
	if (bIsJustDead)
	{
		if (GameplayAbilities_Death.Contains(DamageType))
		{
			DamageAbilityClass = GameplayAbilities_Death[DamageType];
		}
	}
	// 패리
	else if (bIsParrying)
	{
		if (bIsFallingLocally)
		{
			if (GameplayAbilities_ParryMomentumAir.Contains(DamageType))
			{
				DamageAbilityClass = GameplayAbilities_ParryMomentumAir[DamageType];
			}
		}
		else
		{
			if (GameplayAbilities_ParryMomentumGround.Contains(DamageType))
			{
				DamageAbilityClass = GameplayAbilities_ParryMomentumGround[DamageType];
			}
		}
	}
	// 데미지
	else
	{
		if (bIsBiggerDamageThanArmor == false)
		{
			DamageAbilityClass = nullptr;
		}
		else
		{
			if (bIsFallingLocally)
			{
				if (GameplayAbilities_DamageAir.Contains(DamageType))
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
		}
	}

	if (GetLocalRole() == ENetRole::ROLE_Authority
		|| GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
	{
		ServerRPC_ApplyDamageMomentum(NewDirection, DamageAbilityClass);
	}
}

//USceneComponent* AUSACharacterBase::GetDamageMesh()
//{
//	return GetMesh();
//}

void AUSACharacterBase::ApplyDamageHitNiagaraEffect(AController* EventInstigator, AActor* DamageCauser, UNiagaraSystem* SystemTemplate, float OffsetRandomRatioX, float OffsetRandomRatioY, float OffsetRandomRatioZ)
{
	// 팀킬 방지
	// 서버에서만 수행하기 때문에 컨트롤러를 활용할 수 있음
	AAIController* EventAIController = Cast<AAIController>(EventInstigator);
	APlayerController* EventPlayerController = Cast<APlayerController>(EventInstigator);

	AAIController* MyAIController = Cast<AAIController>(GetController());
	APlayerController* MyPlayerController = Cast<APlayerController>(GetController());

	// 같은 팀 (서로 같은 컨트롤러를 검사하는 이유는 스스로 맞는 공격을 구현하기 위함)
	if (MyAIController && EventAIController && (MyAIController != EventAIController))
	{
		return;
	}

	if (MyPlayerController && EventPlayerController && (MyPlayerController != EventPlayerController))
	{
		return;
	}

	MulticastRPC_ApplyDamageHitNiagaraEffect(EventInstigator, DamageCauser, SystemTemplate, OffsetRandomRatioX, OffsetRandomRatioY, OffsetRandomRatioZ);
}

void AUSACharacterBase::MulticastRPC_ApplyDamageHitNiagaraEffect_Implementation(AController* EventInstigator, AActor* DamageCauser, UNiagaraSystem* SystemTemplate, float OffsetRandomRatioX, float OffsetRandomRatioY, float OffsetRandomRatioZ)
{
	if (IsValid(GetCapsuleComponent()) == false)
	{
		return;
	}

	FVector AttachLocalLocation = FVector::ZeroVector;

	if (OffsetRandomRatioX > 0 || OffsetRandomRatioY > 0 || OffsetRandomRatioZ > 0)
	{
		AttachLocalLocation.X = FMath::RandRange(-GetCapsuleComponent()->GetScaledCapsuleRadius(), GetCapsuleComponent()->GetScaledCapsuleRadius());
		AttachLocalLocation.X *= OffsetRandomRatioX;

		AttachLocalLocation.Y = FMath::RandRange(-GetCapsuleComponent()->GetScaledCapsuleRadius(), GetCapsuleComponent()->GetScaledCapsuleRadius());
		AttachLocalLocation.Y *= OffsetRandomRatioY;
		
		AttachLocalLocation.Z = FMath::RandRange(-GetCapsuleComponent()->GetScaledCapsuleHalfHeight(), GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		AttachLocalLocation.Z *= OffsetRandomRatioZ;
	}

	AttachLocalLocation.Z += GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	UNiagaraFunctionLibrary::SpawnSystemAttached(SystemTemplate, GetMesh(), NAME_None, AttachLocalLocation, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
}

//

void AUSACharacterBase::ApplyDamageUSAJellyEffect(TSubclassOf<UDamageType> DamageType)
{
	MulticastRPC_ApplyDamageUSAJellyEffect(DamageType);
}

void AUSACharacterBase::MulticastRPC_ApplyDamageUSAJellyEffect_Implementation(TSubclassOf<UDamageType> DamageType)
{
	if (USAJellyEffectByDamageType.Contains(DamageType) == false)
	{
		return;
	}

	if (IsValid(JellyEffectComponent) == false)
	{
		return;
	}

	JellyEffectComponent->PlayJellyEffect(USAJellyEffectByDamageType[DamageType]);
}

//

void AUSACharacterBase::UpdateCurrentTargetableActor()
{
	// ...
}

void AUSACharacterBase::UpdateCurrentTargetableActor_Instant()
{	
	UpdateCurrentTargetableActor();
}

bool AUSACharacterBase::GetIsTargeting()
{
	return IsValid (CurrentTargetableActor) || IsValid(CurrentTargetableActor_Instant);
}

FVector AUSACharacterBase::GetTargetingDirection()
{
	FVector Result = GetActorForwardVector();

	if (GetIsTargeting() == false)
	{
		return Result;
	}

	if (IsValid(CurrentTargetableActor) == true)
	{
		Result = (CurrentTargetableActor->GetActorLocation() - GetActorLocation());
	}
	else if (IsValid(CurrentTargetableActor_Instant) == true)
	{
		Result = (CurrentTargetableActor_Instant->GetActorLocation() - GetActorLocation());
	}

	Result.Normalize();
	return Result;
}

FVector AUSACharacterBase::GetTargetingDirection2D()
{
	FVector Result = GetActorForwardVector();

	if (GetIsTargeting() == false)
	{
		return Result;
	}

	if (IsValid(CurrentTargetableActor) == true)
	{
		Result = (CurrentTargetableActor->GetActorLocation() - GetActorLocation());
	}
	else if (IsValid(CurrentTargetableActor_Instant) == true)
	{
		Result = (CurrentTargetableActor_Instant->GetActorLocation() - GetActorLocation());
	}

	Result.Z = 0.0f;
	Result.Normalize();
	return Result;
}

FVector AUSACharacterBase::GetTargetableActorLocation()
{
	FVector Result = GetActorLocation();

	if (GetIsTargeting() == false)
	{
		return Result;
	}

	if (IsValid(CurrentTargetableActor) == true)
	{
		Result = CurrentTargetableActor->GetActorLocation();
	}
	else if (IsValid(CurrentTargetableActor_Instant) == true)
	{
		Result = CurrentTargetableActor_Instant->GetActorLocation();
	}

	return Result;
}

IUSATargetableInterface* AUSACharacterBase::GetTargetableInterface()
{
	if (IsValid(CurrentTargetableActor))
	{
		return Cast <IUSATargetableInterface>(CurrentTargetableActor);
	}

	if (IsValid(CurrentTargetableActor_Instant))
	{
		return Cast <IUSATargetableInterface>(CurrentTargetableActor_Instant);
	}

	return nullptr;
}

UUSACharacterAttackComponent* AUSACharacterBase::GetUSACharacterAttackComponent()
{
	return AttackComponent;
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

	if (IsValid(InAbility) == false)
	{
		return;
	}

	// 카메라 예외 처리
	FRotator CameraSpringArmRotation = FRotator::ZeroRotator;
	if (IsValid(CameraSpringArmComponent))
	{
		CameraSpringArmRotation = CameraSpringArmComponent->GetComponentRotation();
	}

	SetActorRotation(InNewDirection.Rotation());
	UpdateComponentTransforms();

	// 카메라 예외 처리
	if (IsValid(CameraSpringArmComponent))
	{
		CameraSpringArmComponent->SetWorldRotation(CameraSpringArmRotation);
	}

	if (/*HasAuthority()*/ UKismetSystemLibrary::IsServer(GetWorld()))
	{
		FGameplayAbilitySpec* GameplayAbilitySpec = ASC->FindAbilitySpecFromClass(InAbility);

		if (GameplayAbilitySpec != nullptr)
		{
			ASC->TryActivateAbility(GameplayAbilitySpec->Handle);
		}
	}
}

void AUSACharacterBase::ChangeCharacterName(const FString& InCharacterName)
{
	ServerRPC_ChangeCharacterName(InCharacterName);
}

void AUSACharacterBase::ServerRPC_ChangeCharacterName_Implementation(const FString& InCharacterName)
{
	CharacterName = InCharacterName;

	OnRep_CharacterName();
}

UAbilitySystemComponent* AUSACharacterBase::GetAbilitySystemComponent() const
{
	return ASC;
}

void AUSACharacterBase::OnRep_ASC()
{	
	//if (IsValid (WeaponDetectBoxComponent) == true)
	//{
	//	WeaponDetectBoxComponent->SetGenerateOverlapEvents(IsValid(ASC));
	//}

	// ... 

	//USA_LOG(LogTemp, Log, TEXT("Hey"));
}

void AUSACharacterBase::OnRep_bIsASCInitialized(bool Prev)
{
	// ...

	//USA_LOG(LogTemp, Log, TEXT("Hey"));
}

void AUSACharacterBase::OnCurrentHealthRatioChanged(float InValue)
{
	K2_OnCurrentHealthRatioChanged
	(
		GetCharacterCurrentHealthRatio_Implementation(),
		GetCharacterMaxHealth_Implementation(),
		GetCharacterCurrentHealth_Implementation()
	);

}

void AUSACharacterBase::OnCurrentHealthRatioChanged(const FOnAttributeChangeData& ChangeData)
{
	K2_OnCurrentHealthRatioChanged
	(
		GetCharacterCurrentHealthRatio_Implementation(),
		GetCharacterMaxHealth_Implementation(),
		GetCharacterCurrentHealth_Implementation()
	);

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

		// 패리 모멘텀 어빌리티
		for (const auto& GameplayDamageAbility : GameplayAbilities_ParryMomentumGround)
		{
			FGameplayAbilitySpec GameplayAbilitySpec(GameplayDamageAbility.Value);
			ASC->GiveAbility(GameplayAbilitySpec);
		}

		for (const auto& GameplayDamageAbility : GameplayAbilities_ParryMomentumAir)
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

		// 리스폰 어빌리티
		FGameplayAbilitySpec GameplayAbilitySpec_Respawn(GameplayAbility_Respawn);
		ASC->GiveAbility(GameplayAbilitySpec_Respawn);

		// 승리 어빌리티
		FGameplayAbilitySpec GameplayAbilitySpec_Win(GameplayAbility_Win);
		ASC->GiveAbility(GameplayAbilitySpec_Win);
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

	RegisteredGameplayTagEvents.Add
	(
		USA_CHARACTER_ADJUST_OFFOVERLAPEVENT,
		ASC->RegisterGameplayTagEvent(USA_CHARACTER_ADJUST_OFFOVERLAPEVENT, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::OnGameplayTagCallback_OffOverlapEvent)
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

	RegisteredGameplayTagEvents.Add
	(
		USA_CHARACTER_ACTION,
		ASC->RegisterGameplayTagEvent(USA_CHARACTER_ACTION, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AUSACharacterBase::OnGameplayTagCallback_Action)
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

		// 리스폰
		FGameplayAbilitySpec* GameplayAbilitySpec_Respawn = ASC->FindAbilitySpecFromClass(GameplayAbility_Respawn);

		if (GameplayAbilitySpec_Respawn)
		{
			if (GameplayAbilitySpec_Respawn->IsActive())
			{
				ASC->AbilitySpecInputPressed(*GameplayAbilitySpec_Respawn);
			}
			else
			{
				ASC->TryActivateAbility(GameplayAbilitySpec_Respawn->Handle);
			}
		}
	}
}

void AUSACharacterBase::SetupAttributeSet()
{
	// 어트리뷰트 설정
	if (ASC != nullptr)
	{
		if (ASC->GetSet <UUSAAttributeSet>() != nullptr)
		{
			ASC->GetGameplayAttributeValueChangeDelegate(UUSAAttributeSet::GetCurrentHealthAttribute()).AddUObject
			(this, &AUSACharacterBase::OnCurrentHealthRatioChanged);

			ASC->GetGameplayAttributeValueChangeDelegate(UUSAAttributeSet::GetMaxHealthAttribute()).AddUObject
			(this, &AUSACharacterBase::OnCurrentHealthRatioChanged);

			ASC->GetSet <UUSAAttributeSet>()->OnCurrentHealthChanged.AddDynamic
			(this, &AUSACharacterBase::OnCurrentHealthRatioChanged);

			ASC->GetSet <UUSAAttributeSet>()->OnMaxHealthChanged.AddDynamic
			(this, &AUSACharacterBase::OnCurrentHealthRatioChanged);
		}
	}
}

void AUSACharacterBase::ResetAttributeSet()
{
	// 어트리뷰트 설정
	CharacterAttributeSetInfo.RenewUSACharacterAttributeSetData(ASC);
}



//void AUSACharacterBase::DieUSACharacter()
//{
//	ServerRPC_OnUSADeath();
//}
//
////void AUSACharacterBase::OnUSADeath()
////{
////	
////}
//
//void AUSACharacterBase::ServerRPC_OnUSADeath_Implementation()
//{
//	MulticastRPC_OnUSADeath();
//}
//
//void AUSACharacterBase::MulticastRPC_OnUSADeath_Implementation()
//{
//	
//}

void AUSACharacterBase::OnRep_CurrentEquipedWeapons(TArray<AUSAWeaponBase*> PrevWeapons)
{
	for (int i = 0; i < PrevWeapons.Num() || i < CurrentEquipedWeapons.Num(); i++)
	{
		if (PrevWeapons[i] != CurrentEquipedWeapons[i])
		{
			if (IsValid(PrevWeapons[i]) == true && IsValid(CurrentEquipedWeapons[i]) == false)
			{
				FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, false);
				PrevWeapons[i]->DetachFromActor(DetachmentTransformRules);

				PrevWeapons[i]->SetWeaponPhysics(true);

				K2_OnUSACurrentWeaponChanged(PrevWeapons[i]->GetWeaponEquipIndex(), nullptr);
			}

			if (IsValid(PrevWeapons[i]) == false && IsValid(CurrentEquipedWeapons[i]) == true)
			{
				CurrentEquipedWeapons[i]->SetWeaponPhysics(false);

				CurrentEquipedWeapons[i]->PlayPickUpAnimationMontageInUSACharacter(ASC, this);

				K2_OnUSACurrentWeaponChanged(CurrentEquipedWeapons[i]->GetWeaponEquipIndex(), CurrentEquipedWeapons[i]);
			}
		}
	}
}

void AUSACharacterBase::OnRep_CurrentOwnedItems(TArray<TSubclassOf<AUSAItemBase>> PrevItems)
{
	// 획득
	if (CurrentOwnedItems.Num() > PrevItems.Num())
	{
		// 항상 뒤로 Add 되니...
		TSubclassOf<AUSAItemBase> TargetItem = CurrentOwnedItems[CurrentOwnedItems.Num() - 1];
		
		if (IsValid(TargetItem) == true
			&& IsValid(TargetItem->GetDefaultObject()) == true)
		{
			Cast<AUSAItemBase>(TargetItem->GetDefaultObject())->PlayPickUpAnimationMontageInUSACharacter(ASC, this);
		}
	}
	// 소모
	//else if (CurrentOwnedItems.Num() < PrevItems.Num())
	//{
	//	if (IsValid(GetCurrentItemClass()) == true
	//		&& IsValid(GetCurrentItemClass()->GetDefaultObject()) == true)
	//	{
	//		Cast<AUSAItemBase>(GetCurrentItemClass()->GetDefaultObject())->PlayUseAnimationMontageInUSACharacter(ASC, this);
	//	}
	//}

	K2_OnCurrentItemCountChanged(GetCurrentItemCount());
}


void AUSACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUSACharacterBase, ASC);
	DOREPLIFETIME(AUSACharacterBase, bIsASCInitialized);
	DOREPLIFETIME(AUSACharacterBase, CurrentEquipedWeapons);
	DOREPLIFETIME(AUSACharacterBase, CurrentOwnedItems);
	DOREPLIFETIME(AUSACharacterBase, CharacterName);

	//DOREPLIFETIME(AUSACharacterBase, StartWeapons);

	//DOREPLIFETIME(AUSACharacterBase, NextWaitingWeapons);
}

