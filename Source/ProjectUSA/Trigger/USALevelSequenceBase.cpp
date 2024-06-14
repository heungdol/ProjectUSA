// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger/USALevelSequenceBase.h"

#include "Components/TextRenderComponent.h"

// Sets default values
AUSALevelSequenceBase::AUSALevelSequenceBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LevelSequenceTriggerTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Component"));
	RootComponent = LevelSequenceTriggerTextComponent;
	LevelSequenceTriggerTextComponent->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	LevelSequenceTriggerTextComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	LevelSequenceTriggerTextComponent->SetText(FText::FromString("Level Sequence Base"));
	LevelSequenceTriggerTextComponent->SetHiddenInGame(true);
}

// Called when the game starts or when spawned
void AUSALevelSequenceBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUSALevelSequenceBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

