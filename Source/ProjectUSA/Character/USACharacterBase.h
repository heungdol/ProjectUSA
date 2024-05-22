// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"
#include "Weapon/USAWeaponBase.h"

#include "Interface/USACharacterInterface.h"
#include "Interface/USATargetableInterface.h"
#include "Interface/USAAttackableInterface.h"

// 어트리뷰트 접근을 위한 헤더
#include "GameplayEffectTypes.h"

// 체크할 태그를 가져오기 위한 헤더
#include "Tag/USAGameplayTags.h"

#include "USACharacterBase.generated.h"

// ========================================================================================

#define KEYNAME_CAPSULEINFO_WALK TEXT("Walk")
#define KEYNAME_CAPSULEINFO_FALL TEXT("Fall")
#define KEYNAME_CAPSULEINFO_CROUCH TEXT("Crouch")


//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSimpleUSACharacterDelegate);
// FCharacterReachedApexSignature

// ========================================================================================

// InputID와 InputAction, GameplayAbility를 관리하기 위한 Struct
USTRUCT(BlueprintType)
struct FUSAGameplayAbilityHandle
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Ability Handle Info")
	int32 InputID = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Ability Handle Info")
	class UInputAction* InputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Ability Handle Info")
	TSubclassOf<class UGameplayAbility> GameplayAbility;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Info")
	//FName GameplayTag;
	
	//FORCEINLINE TObjectPtr <class UInputAction> GetInputAction() { return InputAction;}
};

// ========================================================================================



UENUM(BlueprintType)
enum class EUSACharacterCapsulePivot : uint8
{
	Top UMETA(DisplayName = "Top"),
	Center UMETA(DisplayName = "Center"),
	Bottom UMETA(DisplayName = "Bottom"),
};


USTRUCT(BlueprintType)
struct FUSACharacterCapsuleInfo
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Capsule Info")
	float CapsuleOriginalHalfHeight = 90.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Capsule Info")
	float CapsuleHaflHeight = 90.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Capsule Info")
	float CapsuleRadius = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Capsule Info")
	EUSACharacterCapsulePivot CapsulePivot = EUSACharacterCapsulePivot::Bottom;

public:
	void RenewCharacterCapsule(class ACharacter* InCharacter, class USpringArmComponent* InSpringArmComponent = nullptr);
	void RenewCharacterCapsuleSize(class ACharacter* InCharacter);	
	void RenewCharacterCapsuleLocation(class ACharacter* InCharacter, class USpringArmComponent* InSpringArmComponent = nullptr);

	//

	void RenewJellyEffectMeshLocation(class UUSAJellyEffectComponent* InJellyEffect);
};


// ========================================================================================

USTRUCT(BlueprintType)
struct FUSACharacterAttributeSetInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character AttributeSet Info")
	float StartCurrentHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character AttributeSet Info")
	float StartMaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character AttributeSet Info")
	float StartBaseArmor = 10.0f;

public:
	void RenewUSACharacterAttributeSetData(UAbilitySystemComponent* InASC);

};


// ========================================================================================

USTRUCT(BlueprintType)
struct FUSAGameplayTagInputInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Input GameplayTag Info")
	FGameplayTag InputGameplayTag_Pressed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Input GameplayTag Info")
	FGameplayTag InputGameplayTag_Holding;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Input GameplayTag Info")
	FGameplayTag InputGameplayTag_Released;
};

// ========================================================================================

UCLASS()
class PROJECTUSA_API AUSACharacterBase : public ACharacter, public IAbilitySystemInterface, public IUSACharacterInterface, public IUSATargetableInterface, public IUSAAttackableInterface
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Character Component")
	TObjectPtr <class UCameraComponent> CameraComponent;
	
	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "USA Character Component")
	TObjectPtr <class USpringArmComponent> CameraSpringArmComponent;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Character Component")
	//TObjectPtr <class UStaticMeshComponent> TestStaticMeshComponent;

	//

	FORCEINLINE TObjectPtr <class USpringArmComponent> GetCameraSpringArmComponent() {return CameraSpringArmComponent;}

	//

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Character Component")
	TObjectPtr <class UUSAJellyEffectComponent> JellyEffectComponent;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Character Component")
	//TObjectPtr <class UUSACharacterPivotComponent> PivotComponent;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Input")
	TObjectPtr <class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Input")
	TObjectPtr <class UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Input")
	TObjectPtr <class UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Input")
	TObjectPtr <class UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Input")
	TObjectPtr <class UInputAction> TargetAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Input")
	TObjectPtr <class UInputAction> DropAction;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "USA Character Widget")
	TObjectPtr <class UWidgetComponent> NicknameWidgetComponent;

	UPROPERTY(/*ReplicatedUsing = OnRep_HealthBarWidgetComponent, */EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Widget")
	TObjectPtr <class UWidgetComponent> HealthBarWidgetComponent;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Capsule Info")
	TMap<FName, FUSACharacterCapsuleInfo> CharacterCapsuleInfos;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character AttributeSet Info")
	FUSACharacterAttributeSetInfo CharacterAttributeSetInfo;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "USA Character Weapon")
	TObjectPtr <class UBoxComponent> WeaponDetectBoxComponent;

	FTimerHandle WeaponDetectBoxComponentTimerHandle;

	const float WeaponDetectBoxComponentActiveDelay = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Weapon")
	TMap<EUSAWeaponType, TObjectPtr <class AUSAWeaponBase>> CurrentEquipedWeapons;

	//

	UPROPERTY(ReplicatedUsing = OnRep_StartWeapons, EditDefaultsOnly, BlueprintReadWrite)
	TArray<TObjectPtr <class AUSAWeaponBase>> StartWeapons;

	UPROPERTY()
	bool bIsStartWeaponsInitted = false ;

	UFUNCTION()
	void OnRep_StartWeapons();

	UFUNCTION(BlueprintCallable)
	void AddStartWeapon (class AUSAWeaponBase* InWeapon);

	UFUNCTION()
	void UpdateCurrentWeaponsFromStart();



	//UPROPERTY()
	//bool bIsSetStartWeaponBeforeGASSetup = false;

	//

	FVector USACharacterInputMovementDirection ;

	//
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Targeting Info")
	FName TargetablePivotName = TEXT ("spine_04");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Targeting Info")
	float TargetableActorRange = 1000.0f; 
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Targeting Info")
	TObjectPtr<AActor> CurrentTargetableActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Targeting Info")
	TObjectPtr<AActor> CurrentTargetableActor_Instant;

	//

public:
	// Sets default values for this character's properties
	AUSACharacterBase();

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	virtual void Destroyed() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginDestroy() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	//

	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;

	//

	virtual void Falling() override;

	virtual void Landed(const FHitResult& Hit) override;

public:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPC_RenewCharacterCapsule(/*class ACharacter* InCharacter, */const FName& InKeyName);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_RenewCharacterCapsule(/*class ACharacter* InCharacter, */const FName& InKeyName);

	//

	virtual float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None) override;

	UFUNCTION(Server, Reliable/*, WithValidation*/)
	void ServerRPC_PlayAnimMontage (class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None);

	//

	virtual void StopAnimMontage(class UAnimMontage* AnimMontage = nullptr) override;

	UFUNCTION(Server, Reliable/*, WithValidation*/)
	void ServerRPC_StopAnimMontage(class UAnimMontage* AnimMontage = nullptr);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_StopAnimMontage(class UAnimMontage* AnimMontage = nullptr);

	//

	void OnUSACrouch();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnUSACrouch", ScriptName = "OnUSACrouch"))
	void K2_OnUSACrouch();

	UFUNCTION(Server, Reliable)
	void ServerRPC_OnUSACrouch();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_OnUSACrouch();

	//

	void OnUSAUnCrouch();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnUSAUnCrouch", ScriptName = "OnUSAUnCrouch"))
	void K2_OnUSAUnCrouch();

	UFUNCTION(Server, Reliable)
	void ServerRPC_OnUSAUnCrouch();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_OnUSAUnCrouch();

	//

	UFUNCTION()
	void DieUSACharacter();

	//UFUNCTION()
	//void OnUSADeath();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnUSADeath", ScriptName = "OnUSADeath"))
	void K2_OnUSADeath();

	UFUNCTION(Server, Reliable)
	void ServerRPC_OnUSADeath();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_OnUSADeath();

	//

	float GetCharacterCurrentHealth_Implementation() override;
	float GetCharacterMaxHealth_Implementation() override;
	float GetCharacterCurrentArmor_Implementation() override;
	float GetCharacterCurrentHealthRatio_Implementation() override;

	//
	
	UFUNCTION()
	FVector GetUSACharacterDirection_InputMovement();

	UFUNCTION()
	FVector GetUSACharacterDirection_Target ();

	virtual void UpdateCurrentTargetableActor_Instant();
	virtual void UpdateCurrentTargetableActor();

	virtual bool GetIsTargeting() override;
	virtual FVector GetTargetingDirection() override;
	virtual FVector GetTargetingDirection2D() override;
	virtual FVector GetTargetableActorLocation() override;
	virtual IUSATargetableInterface* GetTargetableInterface() override;

	//

	virtual void StartCameraShake_HitSuccess(TSubclassOf<class UDamageType> DamageType);

	//

	// Weapon의 OnRep을 통해 호출
	void SetCurrentWeapon(EUSAWeaponType InWeaponType, class AUSAWeaponBase* InWeapon);

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHandSocket(class AUSAWeaponBase* InWeapon);

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHolderSocket(class AUSAWeaponBase* InWeapon);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnUSACurrentWeaponChanged", ScriptName = "OnUSACurrentWeaponChanged"))
	void K2_OnUSACurrentWeaponChanged(EUSAWeaponType InType, class AUSAWeaponBase* InWeapon);
	
	//


protected:
	virtual void Move(const struct FInputActionValue& Value);
	virtual void MoveEnd(const struct FInputActionValue& Value);
	virtual void Look(const struct FInputActionValue& Value);
	virtual void DoTarget(const struct FInputActionValue& Value);
	virtual void DoDrop(const struct FInputActionValue& Value);


	UFUNCTION()
	virtual void OnWeaponDetectBoxOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPC_ApplyDamageMomentum(const FVector& InNewDirection, TSubclassOf<UGameplayAbility> InAbility);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_ApplyDamageMomentum(const FVector& InNewDirection, TSubclassOf<UGameplayAbility> InAbility);

	virtual void ApplyDamageMomentum(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser) override;

	//

	//void SetCurrentTargetableActorNullptr();

	//

	void PickupWeapon(class AUSAWeaponBase* InWeapon);

	//UFUNCTION(Server, Reliable)
	//void ServerRPC_PickupWeapon(class AUSAWeaponBase* InWeapon);

	//UFUNCTION(NetMulticast, Reliable)
	//void MulticastRPC_PickupWeapon(class AUSAWeaponBase* InWeapon);


	//

	void DropWeapons(bool bIsAbsolute = false);

	UFUNCTION(Server, Reliable)
	void ServerRPC_DropWeapons();
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_DropWeapons();


	//

	virtual bool GetIsTargetableCurrently() override;

	virtual FVector GetTargetablePivotlocation() override;

	virtual float GetTargetableCapsuleRadius() override;

// Gameplay Abiltiy System Section...

public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// For Player State
	UFUNCTION()
	virtual void OnRep_ASC();

	UPROPERTY(ReplicatedUsing = OnRep_ASC, EditDefaultsOnly, Category = "Character GAS")
	TObjectPtr <class UAbilitySystemComponent> ASC;

	// For AI
	UFUNCTION()
	virtual void OnRep_bIsASCInitialized();

	UPROPERTY(ReplicatedUsing = OnRep_bIsASCInitialized, VisibleAnywhere, Category = "Character GAS")
	bool bIsASCInitialized = false;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character GAS")
	TArray <TSubclassOf<class UGameplayAbility>> GameplayAbilities_Start;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character GAS")
	TArray <TSubclassOf<class UGameplayAbility>> GameplayAbilities_Trigger;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character GAS")
	TArray <FUSAGameplayAbilityHandle> GameplayAbilities_Active;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character GAS")
	TMap <class UInputAction*, FUSAGameplayTagInputInfo> GameplayTagInputInfos;

	TMap <class UInputAction*, bool> bIsCurrentInputPressedMap;

	TMap <int32, bool> bIsCurrentInputPressedIDMap;

	TMap <class UInputAction*, FTimerHandle> CurrentInputTimerHandleMap;

	const float CurrentInputMaintainTime = 0.3f;


	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character GAS")
	TMap <TSubclassOf<class UDamageType>, TSubclassOf<class UGameplayAbility>> GameplayAbilities_DamageGround;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character GAS")
	TMap <TSubclassOf<class UDamageType>, TSubclassOf<class UGameplayAbility>> GameplayAbilities_DamageAir;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character GAS")
	TMap <TSubclassOf<class UDamageType>, TSubclassOf<class UGameplayAbility>> GameplayAbilities_ParryMomentumGround;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character GAS")
	TMap <TSubclassOf<class UDamageType>, TSubclassOf<class UGameplayAbility>> GameplayAbilities_ParryMomentumAir;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character GAS")
	TMap <TSubclassOf<class UDamageType>, TSubclassOf<class UGameplayAbility>> GameplayAbilities_Death;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character GAS")
	TArray <TSubclassOf<class UGameplayAbility>> GameplayAbilities_ETC;

	
	// 중간 접속 시, 태그를 이용하여 캐릭터 상태를 갱신하고자 함
	//const TArray <FGameplayTag> CheckGameplayTagList =
	//{
	//	USA_CHARACTER_STATE_CROUCH,
	//	USA_CHARACTER_STATE_WALK,
	//	USA_CHARACTER_STATE_RUN,
	//	USA_CHARACTER_STATE_FALL,
	//	USA_CHARACTER_STATE_DEAD,
	//	USA_CHARACTER_ACTION_SLIDE
	//};

	//UPROPERTY()
	TMap<FGameplayTag, FDelegateHandle> RegisteredGameplayTagEvents;

	//bool bIsAction;

	//bool bIsSliding;

	//bool bIsFalling;

	//bool bIsWalking;

public:
	void TryGameplayAbilityByGameplayTag(FName GameplayTag);

	UFUNCTION()
	void OnCurrentHealthRatioChanged(float InValue);
	void OnCurrentHealthRatioChanged(const FOnAttributeChangeData& ChangeData);
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnUSACurrentHealthRatioChanged", ScriptName = "OnUSACurrentHealthRatioChanged"))
	void K2_OnCurrentHealthRatioChanged(float InValue);


protected:
	virtual void SetupGAS();
	virtual void PostSetupGAS();

	void SetupAttributeSet();
	void ResetAttributeSet();

	void BeginStartAbilities();

	void InputPressGameplayAbilityByInputID(int32 InputID);
	void InputReleaseGameplayAbilityByInputID(int32 InputID);

	void ActiveGameplayTagInput_Pressed(class UInputAction* InInput);
	void ActiveGameplayTagInput_Released(class UInputAction* InInput);

	UFUNCTION(Server, Reliable)
	void ServerRPC_AddRemovedGameplayTag(const FGameplayTag InTag, bool InAdded = true);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_AddRemovedGameplayTag(const FGameplayTag InTag, bool InAdded = true);


	void OnGameplayTagCallback_IgnoreRotateToMove(const struct FGameplayTag CallbackTag, int32 NewCount);
	void OnGameplayTagCallback_IgnoreMoveInput(const struct FGameplayTag CallbackTag, int32 NewCount);
	void OnGameplayTagCallback_VelocityZero(const struct FGameplayTag CallbackTag, int32 NewCount);
	void OnGameplayTagCallback_CanNotWalkOffLedge(const struct FGameplayTag CallbackTag, int32 NewCount);

	void OnGameplayTagCallback_Walk(const struct FGameplayTag CallbackTag, int32 NewCount);
	void OnGameplayTagCallback_Fall(const struct FGameplayTag CallbackTag, int32 NewCount);
	void OnGameplayTagCallback_Slide(const struct FGameplayTag CallbackTag, int32 NewCount);
	void OnGameplayTagCallback_Crouch(const struct FGameplayTag CallbackTag, int32 NewCount);
	void OnGameplayTagCallback_Dead(const struct FGameplayTag CallbackTag, int32 NewCount);
	void OnGameplayTagCallback_Action(const struct FGameplayTag CallbackTag, int32 NewCount);

	void OnGameplayTagCallback_HandFirstWeapon(const struct FGameplayTag CallbackTag, int32 NewCount);
	void OnGameplayTagCallback_HandSecondWeapon(const struct FGameplayTag CallbackTag, int32 NewCount);

	virtual void CheckCharacterByGameplayTags();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; 

};
