// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interface/USAPickableInterface.h"

#include "Enum/USAEnums.h"

#include "USAWeaponBase.generated.h"

// ========================================================================

UCLASS()
class PROJECTUSA_API AUSAWeaponBase : public AActor, public IUSAPickableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSAWeaponBase();

public:
	virtual void OnConstruction(const FTransform& Transform) override;
	
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Info")
	//TObjectPtr <class USphereComponent> WeaponCollisionComponent;
	
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Info")
	//TObjectPtr <class UBoxComponent> WeaponBoxComponent;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Info")
	//TObjectPtr <class UMeshComponent> WeaponMeshComponent;

	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Weapon Info")
	//TObjectPtr <class UMesh> WeaponMeshRef;

	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Weapon Info")
	//TObjectPtr <class USkeletalMeshComponent> WeaponMesh;

	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Weapon Info")
	//TObjectPtr <class UStaticMeshComponent> WeaponStaticMesh;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "USA Weapon Info")
	TObjectPtr<class USkeletalMesh> WeaponDisplaySkeletalMeshMeshRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "USA Weapon Info")
	TObjectPtr<class UStaticMesh> WeaponDisplayStaticMeshRef;


	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "USA Weapon Info")
	TObjectPtr <class UTexture2D> WeaponTexture;

	//

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Info")
	//TObjectPtr <class URotatingMovementComponent> WeaponRotatingMovementCompont;
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Weapon Info")
	EUSAWeaponType WeaponType = EUSAWeaponType::First;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Weapon Info")
	TArray <TSubclassOf<class UGameplayAbility>> GameplayWeaponAbilities;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Weapon Info")
	FName WeaponHolderSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Weapon Info")
	FName WeaponHandSocketName;

	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Weapon Info")
	TObjectPtr <UAnimMontage> WeaponPickUpAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Weapon Info")
	float WeaponPickUpAnimMontageRate = 1.0f;

	//

	UPROPERTY(ReplicatedUsing = OnRep_PickableActorOwner, VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Info")
	class AUSACharacterBase* PickableActorOwner = nullptr;

	//
	//UPROPERTY(VisibleAnywhere, Category = "Weapon Drop")
	//bool bIsDropping = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "USA Weapon Drop Info")
	bool bIsRandomDrop = true;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "USA Weapon Drop Info")
	float WeaponDropImpulseXY = 300.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "USA Weapon Drop Info")
	float WeaponDropImpulseZ = 300.0f;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon Drop Info")
	//FRotator WeaponIdleRotationRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "USA Weapon Drop Info")
	float bIsWeaponDropping = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//

	//UFUNCTION()
	//void OnRep_WeaponOwnerASC(class UAbilitySystemComponent* PrevASC);



	UFUNCTION(BlueprintCallable)
	bool GiveGameplayWeaponAbilitesToASC(class AUSACharacterBase* InCharacter);

	UFUNCTION(BlueprintCallable)
	bool ClearGameplayWeaponAbilitesToASC(class AUSACharacterBase* InCharacter);

	//UFUNCTION()
	//void OnBoxComponentHitAndCheckIsGround (UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//

	virtual void PickUpByUSACharacter(class UAbilitySystemComponent* InASC, class AUSACharacterBase* InCharacter);

	virtual void PlayPickUpAnimationMontageInUSACharacter(class UAbilitySystemComponent* InASC, class AUSACharacterBase* InCharacter);

	virtual void SetPickableActorOwner(class AUSACharacterBase* InCharacter);

	UFUNCTION()
	void OnRep_PickableActorOwner(class AUSACharacterBase* PrevCharacter);

	//virtual void DropDownFromUSACharacter(class UAbilitySystemComponent* InASC, class AUSACharacterBase* InCharacter);

	//

	//void SetWeaponOwner(class AUSACharacterBase* InCharacter);

	virtual void SetWeaponPhysics(bool IsDropping, bool IsFirst = false);

	//UFUNCTION(NetMulticast, Unreliable)
	//virtual void MulticastRPC_ImpulseWeapon(const FVector& InImpulse);

	//FORCEINLINE class AUSACharacterBase* GetWeaponOwner() { return PickableActorOwner; }

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

	//UFUNCTION(BlueprintCallable)
	//FORCEINLINE  class UStaticMeshComponent* GetStaticMeshComponent() const {return WeaponStaticMesh;}

	//UFUNCTION(BlueprintCallable)
	//FORCEINLINE  class USkeletalMeshComponent* GetSkeletalMeshComponent() const { return WeaponMesh; }

};
