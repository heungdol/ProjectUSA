// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"
#include "Weapon/USAWeaponBase.h"

#include "Interface/USACharacterInterface.h"
#include "Interface/USATargetableInterface.h"
#include "Interface/USAAttackableInterface.h"
#include "Interface/USADamageableInterface.h"
#include "Interface/USAPickableInterface.h"

// 어트리뷰트 접근을 위한 헤더
#include "GameplayEffectTypes.h"

// 체크할 태그를 가져오기 위한 헤더
#include "Tag/USAGameplayTags.h"

#include "Enum/USAEnums.h"
#include "Struct/USAStructs.h"

#include "USACharacterBase.generated.h"

// ========================================================================================

#define KEYNAME_CAPSULEINFO_WALK TEXT("Walk")
#define KEYNAME_CAPSULEINFO_FALL TEXT("Fall")
#define KEYNAME_CAPSULEINFO_CROUCH TEXT("Crouch")


//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSimpleUSACharacterDelegate);
// FCharacterReachedApexSignature

UCLASS()
class PROJECTUSA_API AUSACharacterBase : public ACharacter, 
	public IAbilitySystemInterface, 
	public IUSACharacterInterface, 
	public IUSATargetableInterface, 
	public IUSAAttackableInterface, 
	public IUSADamageableInterface
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
	TObjectPtr <class UUSACharacterAttackComponent> AttackComponent;

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
	TObjectPtr <class UInputAction> ItemAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Input")
	TObjectPtr <class UInputAction> PrevItemAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Input")
	TObjectPtr <class UInputAction> NextItemAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Input")
	TObjectPtr <class UInputAction> DropAction;

	//

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "USA Character Widget")
	//TObjectPtr <class UWidgetComponent> NicknameWidgetComponent;

	UPROPERTY(/*ReplicatedUsing = OnRep_HealthBarWidgetComponent, */EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Widget")
	TObjectPtr <class UWidgetComponent> HealthBarWidgetComponent;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Capsule Info")
	TMap<FName, FUSACharacterCapsuleInfo> CharacterCapsuleInfos;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character AttributeSet Info")
	FUSACharacterAttributeSetInfo CharacterAttributeSetInfo;

	//

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "USA Character Weapon")
	//TObjectPtr <class UBoxComponent> WeaponDetectBoxComponent;

	//FTimerHandle WeaponDetectBoxComponentTimerHandle;

	//const float WeaponDetectBoxComponentActiveDelay = 1.0f;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentEquipedWeapons, VisibleAnywhere, BlueprintReadOnly, Category = "USA Character Weapon")
	TArray<TObjectPtr <class AUSAWeaponBase>> CurrentEquipedWeapons;

	UFUNCTION()
	void OnRep_CurrentEquipedWeapons(TArray<class AUSAWeaponBase*> PrevWeapons);

	//

	UPROPERTY(/*ReplicatedUsing = OnRep_StartWeapons, */EditDefaultsOnly, BlueprintReadWrite, Category = "USA Character Weapon")
	TArray<TSubclassOf <class AUSAWeaponBase>> StartWeaponClassList;

	//

	// 간이 아이템 인벤토리
	UPROPERTY(ReplicatedUsing = OnRep_CurrentOwnedItems, VisibleAnywhere, BlueprintReadOnly, Category = "USA Character Item")
	TArray<TSubclassOf<class AUSAItemBase>> CurrentOwnedItems;

	UFUNCTION()
	void OnRep_CurrentOwnedItems(TArray<TSubclassOf<class AUSAItemBase>> PrevItems);



	//UFUNCTION()
	//void OnRep_StartWeapons();

	//UFUNCTION(BlueprintCallable)
	//void 
	//  (class AUSAWeaponBase* InWeapon);

	//UFUNCTION()
	//void UpdateCurrentWeaponsFromStart();



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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "USA Character Info")
	TObjectPtr<class USkeletalMesh> CharacterDisplaySkeletalMeshMeshRef;

	UPROPERTY(ReplicatedUsing=OnRep_CharacterName, EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Info")
	FString CharacterName = TEXT("Character");

	FORCEINLINE FString GetCharacterName() {return CharacterName;}

	UFUNCTION()
	void OnRep_CharacterName() ;


public:
	// Sets default values for this character's properties
	AUSACharacterBase();

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	virtual void Tick(float DeltaTime) override;

	virtual void BeginDestroy() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;

	virtual void Falling() override;

	virtual void Landed(const FHitResult& Hit) override;

	virtual void SetPlayerDefaults() override;

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

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnUSADeath", ScriptName = "OnUSA"))
	void K2_OnUSADeath();

	//UFUNCTION()
	////void DieUSACharacter();
	//
	//UFUNCTION(Server, Reliable)
	//void ServerRPC_OnUSADeath();

	//UFUNCTION(NetMulticast, Reliable)
	//void MulticastRPC_OnUSADeath();

	//

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnUSARespawn", ScriptName = "OnUSA"))
	void K2_OnUSARespawn();

	UFUNCTION()
	void RespawnUSACharacter();

	UFUNCTION(Server, Reliable)
	void ServerRPC_RespawnUSACharacter();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_RespawnUSACharacter();

	//


	//UFUNCTION()
	//void SetWeaponDetectBoxComponentActive(bool InActive);

	//UFUNCTION(Server, Reliable)
	//void ServerRPC_SetWeaponDetectBoxComponentActive(bool InActive);

	//UFUNCTION(NetMulticast, Reliable)
	//void MulticastRPC_SetWeaponDetectBoxComponentActive(bool InActive);
	
	//

	void SetCurrentWeaponsUsingStartWeaponClassList();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetCurrentWeaponsUsingStartWeaponClassList();

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
	virtual class UUSACharacterAttackComponent* GetUSACharacterAttackComponent() override;
	//

	virtual void StartCameraShake_HitSuccess(TSubclassOf<class UDamageType> DamageType);

	//

	// Weapon의 OnRep을 통해 호출
	bool SetCurrentWeapon(int32 InEquipIndex, class AUSAWeaponBase* InWeapon);

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHandSocket(class AUSAWeaponBase* InWeapon);

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHolderSocket(class AUSAWeaponBase* InWeapon);

	UFUNCTION(BlueprintCallable)
	void AttachAllWeaponToHolderSocket();

	UFUNCTION(BlueprintCallable)
	bool GetIsThereAnyWeapon();

	UFUNCTION(BlueprintCallable)
	void DropWeapons(bool bIsAbsolute = false);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnUSACurrentWeaponChanged", ScriptName = "OnUSACurrentWeaponChanged"))
	void K2_OnUSACurrentWeaponChanged(int32 InEquipIndex, class AUSAWeaponBase* InWeapon);

	//

	//UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnUSAUpdateHUDUserWidget", ScriptName = "OnUSAUpdateHUDUserWidget"))
	//void K2_OnUSAUpdateHUDUserWidget(class UUserWidget* InUserWidget);

	//

	UFUNCTION()
	void USACharacterAnimInstanceMontageNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	//

	UFUNCTION(BlueprintCallable)
	virtual bool PreUseItem();

	UFUNCTION(BlueprintCallable)
	virtual bool PostUseItem();
	
	//UFUNCTION(BlueprintCallable)
	virtual void ChangeItem(bool InIsChangeToNext);

	UFUNCTION(BlueprintPure)
	TSubclassOf<class AUSAItemBase> GetCurrentItemClass();
	
	UFUNCTION(BlueprintPure)
	int32 GetCurrentItemCount();

	//

	UPROPERTY()
	TArray<TObjectPtr<UMaterialInstanceDynamic>> MaterialInstanceDynamicList;

	UFUNCTION(BlueprintCallable)
	void UpdateMaterialParameter (int InIndex, FName InName, float InValue);

	//

	UFUNCTION(BlueprintCallable)
	void ChangeCharacterName(const FString& InCharacterName);

	UFUNCTION(Server, Reliable)
	void ServerRPC_ChangeCharacterName(const FString& InCharacterName);

	//

protected:
	virtual void Move(const struct FInputActionValue& Value);
	virtual void MoveEnd(const struct FInputActionValue& Value);

	virtual void Look(const struct FInputActionValue& Value);
	virtual void LookTarget(const struct FInputActionValue& Value);

	//

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(Client, Reliable)
	void ClientRPC_TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPC_ApplyDamageMomentum(const FVector& InNewDirection, TSubclassOf<UGameplayAbility> InAbility);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_ApplyDamageMomentum(const FVector& InNewDirection, TSubclassOf<UGameplayAbility> InAbility);

	virtual void ApplyDamageMomentum(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser) override;

	//

	UFUNCTION()
	virtual void OnPickableDetectBoxOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void PickUpSomething(IUSAPickableInterface* InPick);

	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "Character GAS")
	bool bIsPickable = false;


	//

	UFUNCTION(Server, Reliable)
	void ServerRPC_DropWeapons();
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_DropWeapons();


	//

	virtual bool GetIsTargetableCurrently() override;

	virtual FVector GetTargetablePivotlocation() override;

	virtual float GetTargetableCapsuleRadius() override;

	//

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnCurrentItemOrderIndexChanged(TSubclassOf<class AUSAItemBase> InItemClass);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnCurrentItemCountChanged(int32 InItemCount);

	//UPROPERTY()
	//bool bIsUsingItem = false;

	//

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnCharacterNameChanged();


// Gameplay Abiltiy System Section
public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// For Player State
	UFUNCTION()
	virtual void OnRep_ASC();

	UPROPERTY(ReplicatedUsing = OnRep_ASC, EditDefaultsOnly, Category = "Character GAS")
	TObjectPtr <class UAbilitySystemComponent> ASC;

	// For AI
	UFUNCTION()
	virtual void OnRep_bIsASCInitialized(bool Prev);

	UPROPERTY(ReplicatedUsing = OnRep_bIsASCInitialized, VisibleAnywhere, Category = "Character GAS")
	bool bIsASCInitialized = false;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character GAS")
	TArray <TSubclassOf<class UGameplayAbility>> GameplayAbilities_Start;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character GAS")
	TArray <TSubclassOf<class UGameplayAbility>> GameplayAbilities_Trigger;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character GAS")
	TArray <FUSAGameplayAbilityHandle> GameplayAbilities_Active;

	// GameplayTag를 직접 붙이는 과정은 보류

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character GAS")
	//TMap <class UInputAction*, FUSAGameplayTagInputInfo> GameplayTagInputInfos;

	//TMap <class UInputAction*, bool> bIsCurrentInputPressedMap;

	//TMap <int32, bool> bIsCurrentInputPressedIDMap;

	//TMap <class UInputAction*, FTimerHandle> CurrentInputTimerHandleMap;

	//const float CurrentInputMaintainTime = 0.3f;


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
	TSubclassOf<class UGameplayAbility> GameplayAbility_Respawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character GAS")
	TMap <TSubclassOf<class UDamageType>, TSubclassOf<class UGameplayAbility>> GameplayAbilities_Death;

	//

	UPROPERTY()
	TSubclassOf<class UDamageType> USADamageType_Explosion;
	
	UPROPERTY()
	TSubclassOf<class UDamageType> USADamageType_Grab;

	UPROPERTY()
	TArray<TSubclassOf<class AUSAItemBase>> USAItemOrder;
	
	UPROPERTY()
	int32 CurrentUSAItemOrderIndex; 


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character GAS")
	FVector ActionCustomLocation;

	FORCEINLINE FVector GetActionCustomLocation() {return ActionCustomLocation;}

	//

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
	void OnGameplayTagCallback_OffOverlapEvent(const struct FGameplayTag CallbackTag, int32 NewCount);


	void OnGameplayTagCallback_Walk(const struct FGameplayTag CallbackTag, int32 NewCount);
	void OnGameplayTagCallback_Fall(const struct FGameplayTag CallbackTag, int32 NewCount);
	void OnGameplayTagCallback_Slide(const struct FGameplayTag CallbackTag, int32 NewCount);
	void OnGameplayTagCallback_Crouch(const struct FGameplayTag CallbackTag, int32 NewCount);
	void OnGameplayTagCallback_Action(const struct FGameplayTag CallbackTag, int32 NewCount);
	virtual void OnGameplayTagCallback_Dead(const struct FGameplayTag CallbackTag, int32 NewCount);


	virtual void CheckCharacterByGameplayTags();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; 

	//

	friend class AUSAWeaponBase;
	friend class AUSAItemBase;
};
