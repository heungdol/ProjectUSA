// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

//#include "AbilitySystemInterface.h"

#include "USAWeaponBase.generated.h"

UENUM(BlueprintType)
enum class EUSAWeaponType : uint8
{
	None UMETA(DisplayName = "None"),
	First UMETA(DisplayName = "First"),
	Second UMETA(DisplayName = "Second"),
};



UCLASS()
class PROJECTUSA_API AUSAWeaponBase : public AActor/*, public IAbilitySystemInterface*/
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSAWeaponBase();

public:
	//UPROPERTY(EditDefaultsOnly, Category = "Weapon Info")
	//TObjectPtr <class UAbilitySystemComponent> ASC;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Info")
	TObjectPtr <class USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Info")
	TArray <TSubclassOf<class UGameplayAbility>> GameplayWeaponAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Info")
	FName WeaponHolderSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Info")
	FName WeaponHandSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Info")
	EUSAWeaponType WeaponType = EUSAWeaponType::First;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//UFUNCTION(BlueprintCallable)
	//void SetAbilitySystemComponent(class UAbilitySystemComponent* InASC);

	//void EquipWeapon();

	//void UnequipWeapon();

	UFUNCTION(BlueprintCallable)
	void GiveGameplayWeaponAbilitesToASC(class UAbilitySystemComponent* InASC);

	UFUNCTION(BlueprintCallable)
	void ClearGameplayWeaponAbilitesToASC(class UAbilitySystemComponent* InASC);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FName GetWeaponHolderSocketName() const {return WeaponHolderSocketName;}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FName GetWeaponHandSocketName() const { return WeaponHandSocketName; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EUSAWeaponType GetWeaponType() const { return WeaponType; }
};
