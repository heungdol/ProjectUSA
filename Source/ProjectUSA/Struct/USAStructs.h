// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Enum/USAEnums.h"
#include "GameplayTagContainer.h"

#include "USAStructs.generated.h"

// USA Attack

// 공격 정보
USTRUCT(BlueprintType)
struct FAttackTraceInfo
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace Attack Info")
	FVector OffsetTraceLocation = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace Attack Info")
	FVector OffsetTraceEndLocation = FVector::ZeroVector;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trace Attack Info")
	bool bIsDirectToTarget = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trace Attack Info")
	bool bIsPinnedLocation = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trace Attack Info")
	bool bIsPinnedRotation = false;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace Attack Info")
	float AttackTraceRadius = 100.0f;

	// 타이밍
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace Attack Info")
	float AttackTime = -1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace Attack Info")
	float AttackDuration = -1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace Attack Info")
	float AttackDamage = -1.0f;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trace Attack Info")
	TSubclassOf<UDamageType> AttackDamageType;

};

// 공격 정보들을 관리하는 데이터
USTRUCT(BlueprintType)
struct FAttackTraceInfos
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace Attack Data")
	TArray <FAttackTraceInfo> AttackTraceInfos;

};

// USA Character Attack Component에서 활용되는 데이터
USTRUCT(BlueprintType)
struct FAttackTraceSceneInfo
{
	GENERATED_BODY()

public:
	FAttackTraceSceneInfo();

	FAttackTraceSceneInfo
	(UWorld* InWorld, const FAttackTraceInfo& InDefault, AActor* InSource,
		const FVector& InLocation,
		const FVector& InFoward, const FVector& InRight, const FVector& InUp);

	// 카피 후 활용
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trace Attack Scene Info")
	FAttackTraceInfo DefaultAttackTraceInfo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trace Attack Info")
	float StartAttackTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trace Attack Info")
	AActor* AttackSourceActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trace Attack Info")
	TArray <AActor*> CheckedActorList;

	//

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trace Attack Info")
	FVector AttackForwardDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trace Attack Info")
	FVector AttackRightDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trace Attack Info")
	FVector AttackUpDirection;

	//

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trace Attack Info")
	FVector AttackLocation;

public:

	void DoAttackTrace(UWorld* InWorld, float DeltaTime);

	bool GetIsAttackEnded(UWorld* InWorld);
};


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
	void RenewUSACharacterAttributeSetData(class UAbilitySystemComponent* InASC);

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
