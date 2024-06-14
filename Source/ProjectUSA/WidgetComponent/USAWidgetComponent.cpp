// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetComponent/USAWidgetComponent.h"

#include "Player/USAPlayerCameraManager.h"

void UUSAWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

    APlayerController* FirstPlayerController = GetWorld()->GetFirstPlayerController();

    if (IsValid(FirstPlayerController) == true)
    {
        LocalUSAPlayerCameraManager = Cast <AUSAPlayerCameraManager>(FirstPlayerController->PlayerCameraManager);
    }
}

void UUSAWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (IsValid(LocalUSAPlayerCameraManager) == false)
    {
        return;
    }

    if (IsValid(GetWidget()) == false)
    {
        return;
    }

    if (FVector::Distance(LocalUSAPlayerCameraManager->GetCameraLocation(), GetComponentLocation()) > CullDistanceFromUSAPlayerCamera)
    {
        GetWidget()->SetRenderScale(FVector2D(0.0f, 0.0f));
    }
    else
    {
        GetWidget()->SetRenderScale(FVector2D(1.0f, 1.0f));
    }
}

