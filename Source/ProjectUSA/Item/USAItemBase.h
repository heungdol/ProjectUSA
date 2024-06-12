// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interface/USAPickableInterface.h"

#include "USAItemBase.generated.h"

UCLASS()
class PROJECTUSA_API AUSAItemBase : public AActor, public IUSAPickableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUSAItemBase();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Item Info")
	TSubclassOf<AUSAItemBase> ItemSubclass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Item Info")
	TObjectPtr<class USphereComponent> ItemSphereComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Item Info")
	TObjectPtr<class UStaticMeshComponent> ItemStaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "USA Item Info")
	TObjectPtr <class UTexture2D> ItemTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Item Info")
	TObjectPtr <UAnimMontage> ItemPickUpAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Item Info")
	float ItemPickUpAnimMontageRate = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Item Info")
	TObjectPtr <UAnimMontage> ItemUseAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Item Info")
	float ItemUseAnimMontageRate = 1.0f;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "USA Item Info")
	//TSubclassOf<class UGameplayEffect> ItemGameplayEffect;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//

	virtual void PickUpByUSACharacter(class UAbilitySystemComponent* InASC, class AUSACharacterBase* InCharacter);

	virtual void PlayPickUpAnimationMontageInUSACharacter(class UAbilitySystemComponent* InASC, class AUSACharacterBase* InCharacter);
	
	//virtual void SetPickableActorOwner(class AUSACharacterBase* InCharacter);

	//UFUNCTION()
	//void OnRep_PickableActorOwner(class AUSACharacterBase* PrevCharacter);

	//

	UFUNCTION(BlueprintCallable)
	virtual void ActiveUSAItem(class UAbilitySystemComponent* InASC, class AUSACharacterBase* InCharacter);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "ActiveUSAItem"))
	void K2_ActiveUSAItem(class UAbilitySystemComponent* InASC, class AUSACharacterBase* InCharacter);

	UFUNCTION(BlueprintCallable)
	virtual void PlayUseAnimationMontageInUSACharacter(class UAbilitySystemComponent* InASC, class AUSACharacterBase* InCharacter);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class UTexture2D* GetItemTexture() const { return ItemTexture; }

	//TSubclassOf<UGameplayEffect> GetItemGameplayEffect();

};
