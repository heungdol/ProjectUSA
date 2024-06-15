// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/USACheckpoint.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Character/USACharacterPlayer.h"

#include "Player/USAPlayerController.h"

#include "GameMode/USAGameModeBase.h"

#include "Kismet/GameplayStatics.h"

#include "Components/TextRenderComponent.h"

// Sets default values
AUSACheckpoint::AUSACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//CheckpointHolderComponent = CreateDefaultSubobject <USceneComponent>(TEXT("Checkpoint Holder Component"));
	//RootComponent = CheckpointHolderComponent;

	CheckpointBoxComponent = CreateDefaultSubobject <UBoxComponent>(TEXT("Checkpoint Box Component"));
	CheckpointBoxComponent->SetupAttachment(RootComponent);
	CheckpointBoxComponent->SetGenerateOverlapEvents(true);
	CheckpointBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AUSACheckpoint::OnDetectBoxOverlapBegin);
	RootComponent = CheckpointBoxComponent;

	CheckpointStaticMeshComponent = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Checkpoint Static Mesh Component"));
	CheckpointStaticMeshComponent->SetupAttachment(RootComponent);

	CheckpointTextComponent = CreateDefaultSubobject <UTextRenderComponent>(TEXT("Checkpoint Text Render"));
	CheckpointTextComponent->SetupAttachment(RootComponent);

	CheckpointTextComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	CheckpointTextComponent->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	CheckpointTextComponent->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	CheckpointTextComponent->SetHiddenInGame(true);
}

void AUSACheckpoint::OnConstruction(const FTransform& Transform)
{
	if (CheckpointTextComponent)
	{
		CheckpointTextComponent->SetText(FText::Format(FText::FromString(TEXT("Checkpoint Index: {0}")), FText::AsNumber(CheckpointIndex)));
	}
}

// Called when the game starts or when spawned
void AUSACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void AUSACheckpoint::OnDetectBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (IsValid(OtherActor) == false)
	//{
	//	return;
	//}

	AUSACharacterPlayer* USACharacterPlayer = Cast <AUSACharacterPlayer>(OtherActor);

	if (IsValid(USACharacterPlayer) == false)
	{
		return;
	}

	AUSAPlayerController* USAPlayerController = USACharacterPlayer->GetController<AUSAPlayerController>();

	if (IsValid(USAPlayerController) == false)
	{
		return;
	}

	// 서버라면 체크포인트 설정
	AUSAGameModeBase* USAGameModeBase = Cast<AUSAGameModeBase>(GetWorld()->GetAuthGameMode());
	if (IsValid(USAGameModeBase))
	{
		USAGameModeBase->UpdatePlayerControllerCheckpoint(USAPlayerController, CheckpointIndex);
	}

	// 플레이어가 로컬이면 블루프린트 이벤트 호출
	if (USAPlayerController->IsLocalPlayerController())
	{
		TArray<AActor*> GetActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), StaticClass(), GetActors);

		for (AActor* GetActor : GetActors)
		{
			AUSACheckpoint* USACheckpoint = Cast<AUSACheckpoint>(GetActor);
			if (IsValid(USACheckpoint) == false)
			{
				continue;
			}

			USACheckpoint->K2_OnCheckpointUpdated(this == USACheckpoint);
		}
	}
}

// Called every frame
void AUSACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

