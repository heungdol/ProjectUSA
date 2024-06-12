// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/USAPlayerStart.h"

#include "Components/TextRenderComponent.h"

AUSAPlayerStart::AUSAPlayerStart(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerStartTextComponent = CreateDefaultSubobject <UTextRenderComponent>(TEXT("Player Start Text Render"));
	PlayerStartTextComponent->SetupAttachment(RootComponent);

	PlayerStartTextComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	PlayerStartTextComponent->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	PlayerStartTextComponent->SetHiddenInGame(true);
}

void AUSAPlayerStart::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (PlayerStartTextComponent)
	{
		PlayerStartTextComponent->SetText (FText::Format(FText::FromString(TEXT("PlayerStart Index: {0}")), FText::AsNumber(PlayerStartIndex)));
	}
}


