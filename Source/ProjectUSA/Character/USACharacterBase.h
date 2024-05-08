// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"
#include "Weapon/USAWeaponBase.h"

#include "Interface/USACharacterInterface.h"
#include "Interface/USATargetableInterface.h"

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
	void RenewCharacterCapsule(class ACharacter* InCharacter);
	void RenewCharacterCapsuleSize(class ACharacter* InCharacter);	
	void RenewCharacterCapsuleLocation(class ACharacter* InCharacter);

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

UCLASS()
class PROJECTUSA_API AUSACharacterBase : public ACharacter, public IAbilitySystemInterface, public IUSACharacterInterface, public IUSATargetableInterface
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Character Component")
	TObjectPtr <class UCameraComponent> CameraComponent;
	
	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "USA Character Component")
	TObjectPtr <class USpringArmComponent> CameraSpringArmComponent;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Weapon")
	TMap<EUSAWeaponType, TObjectPtr <class AUSAWeaponBase>> CurrentEquipedWeapons;

	UPROPERTY(ReplicatedUsing = OnRep_NextWeapon, EditDefaultsOnly, BlueprintReadWrite)
	TArray<TObjectPtr <class AUSAWeaponBase>> NextWaitingWeapons;

	UPROPERTY()
	bool bIsSetNextWeaponBeforeGASSetup = false;

	//

	FVector USACharacterInputMovementDirection ;

	//
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Targeting Info")
	FName TargetablePivotName = TEXT ("spine_04");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Targeting Info")
	float TargetableActorRange = 1000.0f; 
	
	TObjectPtr<AActor> CurrentTargetableActor;
	//TArray<AActor*> TargetableActors;



public:
	// Sets default values for this character's properties
	AUSACharacterBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	//

	virtual void Falling() override;

	virtual void Landed(const FHitResult& Hit) override;

public:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPC_RenewCharacterCapsule(class ACharacter* InCharacter, const FName& InKeyName);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_RenewCharacterCapsule(class ACharacter* InCharacter, const FName& InKeyName);

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

	UFUNCTION(BlueprintCallable)
	void AddWaitingWeapon(class AUSAWeaponBase* InNextWeapon);

	//UFUNCTION(Server, Reliable, WithValidation)
	//void ServerRPC_SetNextWeapon(class AUSAWeaponBase* InNextWeapon);

	//UFUNCTION(NetMulticast, Reliable)
	//void MulticastRPC_SetNextWeapon(class AUSAWeaponBase* InNextWeapon);

	UFUNCTION()
	void OnRep_NextWeapon();

	//void EquipFinalNextWeapon();

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

protected:
	virtual void Move(const struct FInputActionValue& Value);
	virtual void MoveEnd(const struct FInputActionValue& Value);
	virtual void Look(const struct FInputActionValue& Value);
	virtual void DoTarget(const struct FInputActionValue& Value);

	//

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_TakeDamage(float DamageAmount);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPC_ApplyDamageMomentum(const FVector& InNewDirection, TSubclassOf<UGameplayAbility> InAbility);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_ApplyDamageMomentum(const FVector& InNewDirection, TSubclassOf<UGameplayAbility> InAbility);

	virtual void ApplyDamageMomentum(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser) override;

	//

	virtual void UpdateCurrentTargetableActor();
	virtual void UpdateCurrentTargetableActor_Instant();
	//void SetCurrentTargetableActorNullptr();

	//

	//UFUNCTION(BlueprintCallable)
	void UpdateCurrentWeapons(/*class AUSAWeaponBase* InWeapon*/);
	
	UFUNCTION(BlueprintCallable)
	void UnequipWeapon(/*class AUSAWeaponBase* InWeapon*/EUSAWeaponType InWeaponType);

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHandSocket (class AUSAWeaponBase* InWeapon);

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHolderSocket(class AUSAWeaponBase* InWeapon);


	virtual bool GetIsTargetableCurrently() override;

	virtual FVector GetTargetablePivotlocation() override;


// Gameplay Abiltiy System Secion...

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

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character GAS")
	TMap <TSubclassOf<class UDamageType>, TSubclassOf<class UGameplayAbility>> GameplayAbilities_DamageGround;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character GAS")
	TMap <TSubclassOf<class UDamageType>, TSubclassOf<class UGameplayAbility>> GameplayAbilities_DamageAir;

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

	void OnGameplayTagCallback_IgnoreRotateToMove(const struct FGameplayTag CallbackTag, int32 NewCount);
	void OnGameplayTagCallback_IgnoreMoveInput(const struct FGameplayTag CallbackTag, int32 NewCount);
	void OnGameplayTagCallback_VelocityZero(const struct FGameplayTag CallbackTag, int32 NewCount);
	void OnGameplayTagCallback_CanNotWalkOffLedge(const struct FGameplayTag CallbackTag, int32 NewCount);

	void OnGameplayTagCallback_Walk(const struct FGameplayTag CallbackTag, int32 NewCount);
	void OnGameplayTagCallback_Fall(const struct FGameplayTag CallbackTag, int32 NewCount);
	void OnGameplayTagCallback_Slide(const struct FGameplayTag CallbackTag, int32 NewCount);
	void OnGameplayTagCallback_Crouch(const struct FGameplayTag CallbackTag, int32 NewCount);
	void OnGameplayTagCallback_Dead(const struct FGameplayTag CallbackTag, int32 NewCount);

	void OnGameplayTagCallback_HandFirstWeapon(const struct FGameplayTag CallbackTag, int32 NewCount);
	void OnGameplayTagCallback_HandSecondWeapon(const struct FGameplayTag CallbackTag, int32 NewCount);

	virtual void CheckCharacterByGameplayTags();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; 

};
