// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/USACharacterSpawner.h"

#include "Character/USACharacterBase.h"
#include "Character/USACharacterNonPlayer.h"

#include "GameFramework/PlayerController.h"

#include "Components/ArrowComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/BoxComponent.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"


// Sets default values
AUSACharacterSpawner::AUSACharacterSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterSpawnBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Character Spawn Box Component"));
	RootComponent = CharacterSpawnBoxComponent;
	CharacterSpawnBoxComponent->SetBoxExtent(FVector(1000.0f, 1000.0f, 200.0f), true);
	CharacterSpawnBoxComponent->SetGenerateOverlapEvents(true);
	CharacterSpawnBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AUSACharacterSpawner::OnPlayerDetectBoxOverlapBegin);

	CharacterSpawnTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Character Spawn Text Component"));
	CharacterSpawnTextComponent->SetupAttachment(RootComponent);
	CharacterSpawnTextComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	CharacterSpawnTextComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	CharacterSpawnTextComponent->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	CharacterSpawnTextComponent->SetHiddenInGame(true);
	CharacterSpawnTextComponent->SetText(FText::FromString(TEXT("Spawn USA Characters")));

	//CharacterSpawnArrowComponents.Init(nullptr, CharacterSpawnNum);
	CharacterSpawnSkeletalMeshComponents.Init(nullptr, CharacterSpawnNum);

	for (int Index = 0; Index < CharacterSpawnNum; Index++)
	{
		//FText SpawnArrowName = FText::Format(NSLOCTEXT("", "", "Character Spawn Arrow Component {0}"), Index);
		//CharacterSpawnArrowComponents[Index] = CreateDefaultSubobject<UArrowComponent>(*SpawnArrowName.ToString());
		//CharacterSpawnArrowComponents[Index]->SetupAttachment(RootComponent);
		//CharacterSpawnArrowComponents[Index]->SetHiddenInGame(true);

		FText SpawnSkeletalName = FText::Format(NSLOCTEXT("", "", "Character Spawn Skeletal Mesh Component {0}"), Index);
		CharacterSpawnSkeletalMeshComponents[Index] = CreateDefaultSubobject<USkeletalMeshComponent>(*SpawnSkeletalName.ToString());
		CharacterSpawnSkeletalMeshComponents[Index]->SetupAttachment(RootComponent);
		CharacterSpawnSkeletalMeshComponents[Index]->SetHiddenInGame(true);
	}
}

void AUSACharacterSpawner::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	for (int Index = 0; Index < CharacterSpawnClasses.Num(); Index++)
	{
		if (IsValid(CharacterSpawnSkeletalMeshComponents[Index]) == false
			/*|| IsValid(CharacterSpawnArrowComponents[Index]) == false*/)
		{
			continue;
		}

		if (IsValid(CharacterSpawnClasses[Index]) == false
			|| IsValid(Cast<AUSACharacterNonPlayer>(CharacterSpawnClasses[Index]->GetDefaultObject())) == false)
		{
			CharacterSpawnSkeletalMeshComponents[Index]->SetSkeletalMesh(nullptr);
			continue;
		}

		CharacterSpawnSkeletalMeshComponents[Index]->SetSkeletalMesh(Cast<AUSACharacterNonPlayer>(CharacterSpawnClasses[Index]->GetDefaultObject())->CharacterDisplaySkeletalMeshMeshRef);
		//CharacterSpawnSkeletalMeshComponents[Index]->SetRelativeLocation(CharacterSpawnArrowComponents[Index]->GetRelativeLocation());
		//CharacterSpawnSkeletalMeshComponents[Index]->SetRelativeRotation(CharacterSpawnArrowComponents[Index]->GetRelativeRotation());
	}

}

void AUSACharacterSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	bIsActivated = false;
}

void AUSACharacterSpawner::OnPlayerDetectBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsActivated == true)
	{
		return;
	}

	if (GetWorld()->GetAuthGameMode() == nullptr)
	{
		return;
	}

	APawn* OtherPawn = Cast<APawn>(OtherActor);

	if (IsValid(OtherPawn) == false 
		|| IsValid(OtherPawn->GetController<APlayerController>()) == false)
	{
		return;
	}

	for (int Index = 0; Index < CharacterSpawnClasses.Num(); Index++)
	{
		if (IsValid(CharacterSpawnClasses[Index]) == false
			|| IsValid(Cast<AUSACharacterBase>(CharacterSpawnClasses[Index]->GetDefaultObject())) == false)
		{
			continue;
		}

		if (IsValid(CharacterSpawnSkeletalMeshComponents[Index]) == false)
		{
			continue;
		}

		UAIBlueprintHelperLibrary::SpawnAIFromClass
		(GetWorld(),
			CharacterSpawnClasses[Index],
			Cast<AUSACharacterNonPlayer>(CharacterSpawnClasses[Index]->GetDefaultObject())->StartBehaviorTree,
			CharacterSpawnSkeletalMeshComponents[Index]->GetComponentLocation(),
			CharacterSpawnSkeletalMeshComponents[Index]->GetComponentRotation()
		);
	}

	bIsActivated = true;
}

