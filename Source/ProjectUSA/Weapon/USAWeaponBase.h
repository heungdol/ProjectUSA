// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "USAWeaponBase.generated.h"

// ========================================================================

UENUM(BlueprintType)
enum class EUSAWeaponType : uint8
{
	None UMETA(DisplayName = "None"),
	First UMETA(DisplayName = "First"),
	Second UMETA(DisplayName = "Second"),
};

// ========================================================================

UCLASS()
class PROJECTUSA_API AUSAWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSAWeaponBase();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Info")
	TObjectPtr <class USceneComponent> WeaponMeshHolder;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Info")
	TObjectPtr <class USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Info")
	TObjectPtr <class UStaticMeshComponent> WeaponStaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Info")
	TObjectPtr <class UBoxComponent> WeaponBoxComponent;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Info")
	TObjectPtr <class UTexture2D> WeaponTexture;

	//
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Info")
	EUSAWeaponType WeaponType = EUSAWeaponType::First;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Info")
	TArray <TSubclassOf<class UGameplayAbility>> GameplayWeaponAbilities;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Info")
	FName WeaponHolderSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Info")
	FName WeaponHandSocketName;

	//


	UPROPERTY(ReplicatedUsing = OnRep_WeaponOwnerASC, VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Info")
	class UAbilitySystemComponent* WeaponOwnerASC = nullptr;

	//UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Info")
	//bool bHasWeaponOwener = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//

	UFUNCTION(BlueprintCallable)
	void GiveGameplayWeaponAbilitesToASC(class UAbilitySystemComponent* InASC);

	UFUNCTION(BlueprintCallable)
	void ClearGameplayWeaponAbilitesToASC(/*class UAbilitySystemComponent* InASC*/);

	//UFUNCTION(BlueprintCallable)
	//void ClearWeaponOwner();

	UFUNCTION()
	void OnRep_WeaponOwnerASC();

	//UFUNCTION(BlueprintCallable)
	//void SetWeapon

	//

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FName GetWeaponHolderSocketName() const {return WeaponHolderSocketName;}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FName GetWeaponHandSocketName() const { return WeaponHandSocketName; }

	//

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EUSAWeaponType GetWeaponType() const { return WeaponType; }

	//

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UTexture2D* GetWeaponTexture() const { return WeaponTexture; }

protected:
	UFUNCTION()
	void OnWeaponOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
