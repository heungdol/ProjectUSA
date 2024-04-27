// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"
#include "Weapon/USAWeaponBase.h"

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



// ========================================================================================

UCLASS()
class PROJECTUSA_API AUSACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUSACharacterBase();

	//virtual void OnConstruction(const FTransform& Transform) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Character Component")
	TObjectPtr <class UCameraComponent> CameraComponent;
	
	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "USA Character Component")
	TObjectPtr <class USpringArmComponent> CameraSpringArmComponent;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Character Component")
	TObjectPtr <class UUSAJellyEffectComponent> JellyEffectComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Character Component")
	TObjectPtr <class UUSACharacterPivotComponent> PivotComponent;



	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Input")
	TObjectPtr <class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Input")
	TObjectPtr <class UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Input")
	TObjectPtr <class UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Input")
	TObjectPtr <class UInputAction> LookAction;



	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Capsule Info")
	TMap<FName, FUSACharacterCapsuleInfo> CharacterCapsuleInfos;



	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Weapon")
	TMap<EUSAWeaponType, TObjectPtr <class AUSAWeaponBase>> CurrentEquipedWeapons;


	UPROPERTY(ReplicatedUsing = OnRep_NextWeapon, EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<class AUSAWeaponBase> NextWeapon;


	UPROPERTY()
	bool bIsSetNextWeaponBeforeGASSetup = false;


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
	void SetNextWeapon(class AUSAWeaponBase* InNextWeapon);

	//UFUNCTION(Server, Reliable, WithValidation)
	//void ServerRPC_SetNextWeapon(class AUSAWeaponBase* InNextWeapon);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SetNextWeapon(class AUSAWeaponBase* InNextWeapon);

	UFUNCTION()
	void OnRep_NextWeapon();

	void EquipFinalNextWeapon();

	//


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	//

	virtual void Falling() override;

	virtual void Landed(const FHitResult& Hit) override;

	//

	void OnUSACrouch();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnUSACrouch", ScriptName = "OnUSACrouch"))
	void K2_OnUSACrouch();

	UFUNCTION(Server, Reliable)
	void ServerRPC_OnUSACrouch();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_OnUSACrouch();

	// ...

	void OnUSAUnCrouch();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnUSAUnCrouch", ScriptName = "OnUSAUnCrouch"))
	void K2_OnUSAUnCrouch();

	UFUNCTION(Server, Reliable)
	void ServerRPC_OnUSAUnCrouch();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_OnUSAUnCrouch();

	// ...

	// + Anim Ability task 재확인할 것

protected:
	virtual void Move(const struct FInputActionValue& Value);
	virtual void Look(const struct FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(class AUSAWeaponBase* InWeapon);
	
	UFUNCTION(BlueprintCallable)
	void UnequipWeapon(/*class AUSAWeaponBase* InWeapon*/EUSAWeaponType InWeaponType);

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHandSocket (class AUSAWeaponBase* InWeapon);

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHolderSocket(class AUSAWeaponBase* InWeapon);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override; 

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPC_ApplyDamageMomentum(const FVector& InNewDirection, TSubclassOf<UGameplayAbility> InAbility);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_ApplyDamageMomentum(const FVector& InNewDirection, TSubclassOf<UGameplayAbility> InAbility);

	virtual void ApplyDamageMomentum(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser) override;


public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UFUNCTION()
	void OnRep_ASC();

	UPROPERTY(ReplicatedUsing = OnRep_ASC, EditDefaultsOnly, Category = GAS)
	TObjectPtr <class UAbilitySystemComponent> ASC;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character GAS")
	TArray <TSubclassOf<class UGameplayAbility>> GameplayStartAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character GAS")
	TArray <TSubclassOf<class UGameplayAbility>> GameplayTriggerAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character GAS")
	TArray <FUSAGameplayAbilityHandle> GameplayActiveAbilities;



	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character GAS")
	TMap <TSubclassOf<class UDamageType>, TSubclassOf<class UGameplayAbility>> GameplayDamageGroundAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character GAS")
	TMap <TSubclassOf<class UDamageType>, TSubclassOf<class UGameplayAbility>> GameplayDamageAirAbilities;


public:
	void TryGameplayAbilityByGameplayTag(FName GameplayTag);


protected:
	virtual void SetupGAS();

	virtual void PostSetupGAS();

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

	void OnGameplayTagCallback_HandFirstWeapon(const struct FGameplayTag CallbackTag, int32 NewCount);
	void OnGameplayTagCallback_HandSecondWeapon(const struct FGameplayTag CallbackTag, int32 NewCount);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; 

};
