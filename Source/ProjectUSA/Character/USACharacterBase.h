// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AbilitySystemInterface.h"

#include "USACharacterBase.generated.h"

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


USTRUCT(BlueprintType)
struct FCharacterMovementWalkInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement Info")
	float MaxWalkSpeed = 500;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement Info")
	FRotator RotationRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement Info")
	float GroundFriction = 8;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement Info")
	float BrakingDecelerationWalking = 2000;

public:

	void RenewCharacterMovementInfo(class UCharacterMovementComponent* InMovementComponent);
};


UCLASS()
class PROJECTUSA_API AUSACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUSACharacterBase();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Info")
	float CapsuleHeight = 180.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Info")
	float CapsuleRadius = 20.0f;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Character Component")
	TObjectPtr <class UCameraComponent> CameraComponent;
	
	UPROPERTY (VisibleAnywhere, BlueprintReadOnly, Category = "USA Character Component")
	TObjectPtr <class USpringArmComponent> CameraSpringArmComponent;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Character Component")
	TObjectPtr <class UUSAJellyEffectComponent> JellyEffectComponent;



	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Input")
	TObjectPtr <class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Input")
	TObjectPtr <class UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Input")
	TObjectPtr <class UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Input")
	TObjectPtr <class UInputAction> LookAction;



	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Movement Walk Info")
	FCharacterMovementWalkInfo CharacterMovementWalkInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Character Movement Walk Info")
	FCharacterMovementWalkInfo CharacterMovementSlideInfo;

	UPROPERTY()
	bool bIsVelocityZero = false;

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


protected:
	void Move(const struct FInputActionValue& Value);
	void Look(const struct FInputActionValue& Value);

	void AdjustVelocityWithVelocityZero();

	void InputPressGameplayAbilityByInputID(int32 InputID);
	void InputReleaseGameplayAbilityByInputID(int32 InputID);

	void TryGameplayAbilityByGameplayTag(FName GameplayTag);

	void GameplayTagIgnoreRotateToMoveCallback(const struct FGameplayTag CallbackTag, int32 NewCount);
	void GameplayTagIgnoreMoveInputCallback(const struct FGameplayTag CallbackTag, int32 NewCount);
	void GameplayTagVelocityZeroCallback(const struct FGameplayTag CallbackTag, int32 NewCount);

	void GameplayTagSlideCallback(const struct FGameplayTag CallbackTag, int32 NewCount);

public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	TObjectPtr <class UAbilitySystemComponent> ASC;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character GAS")
	TArray <TSubclassOf<class UGameplayAbility>> GameplayStartAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character GAS")
	TArray <TSubclassOf<class UGameplayAbility>> GameplayTriggerAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character GAS")
	TArray <FUSAGameplayAbilityHandle> GameplayActiveAbilities;


protected:
	virtual void SetupGAS();

};
