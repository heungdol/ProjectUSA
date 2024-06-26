// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/USAJellyEffectComponent.h"

#include "Components/MeshComponent.h"
#include "Components/CapsuleComponent.h"

#include "Data/USAJellyEffectData.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Kismet/KismetMathLibrary.h"

#include "Curves/CurveVector.h"

// Sets default values for this component's properties
UUSAJellyEffectComponent::UUSAJellyEffectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bIsPlayingJellyEffect = false;
}


// Called when the game starts
void UUSAJellyEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	bIsPlayingJellyEffect = false;
}


// Called every frame
void UUSAJellyEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TickJellyEffect();
	TickJellyEffectByGravity();

	TickJellyEffectFinal();
}

void UUSAJellyEffectComponent::SetJellySceneComponent(ACharacter* InCharacter, USceneComponent* InComponent)
{
	JellySceneComponent = InComponent;

	if (InCharacter != nullptr
		&& InCharacter->GetClass() != nullptr
		&& InCharacter->GetClass()->GetDefaultObject<ACharacter>() != nullptr
		&& InCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetMesh() != nullptr)
	{
		StartMeshLocation = InCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetMesh()->GetRelativeLocation();
		StartMeshRotation = InCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetMesh()->GetRelativeRotation();
		StartMeshScale = InCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetMesh()->GetRelativeScale3D();
	}
	else
	{
		StartMeshLocation = FVector::ZeroVector;
		StartMeshRotation = FRotator::ZeroRotator;
		StartMeshScale = FVector::OneVector;
	}
}

void UUSAJellyEffectComponent::SetMeshStartLocation(FVector InVector)
{
	StartMeshLocation = InVector;
}

void UUSAJellyEffectComponent::PlayJellyEffect(UUSAJellyEffectData* InJellyEffectData)
{
	// 젤리 데이터 유효성 판단
	if (IsValid(InJellyEffectData) == false)
	{
		return;
	}

	// 젤리 이펙트 대상 존재 확인
	if (IsValid(GetJellySceneComponent()) == false)
	{
		return;
	}

	// 젤리 데이터 및 시작 설정
	CurrentJellyEffectData = InJellyEffectData;
	bIsPlayingJellyEffect = true;

	// 시작 및 마지막 시간 설정
	PlayJellyEffectTime = GetWorld()->GetTimeSeconds();
	EndJellyEffectTime = PlayJellyEffectTime + CurrentJellyEffectData->GetJellyEffectTime();

	// 젤리 이펙트 수행 (첫 틱 -> 0.0f)
	CurrentJellyEffectLocation = CurrentJellyEffectData->GetLocationVectorByRatio(0.0f);
	CurrentJellyEffectRotation = FRotator::MakeFromEuler(CurrentJellyEffectData->GetRotationVectorByRatio(0.0f));
	CurrentJellyEffectScale = CurrentJellyEffectData->GetScaleVectorByRatio(0.0f);
}

void UUSAJellyEffectComponent::StopJellyEffect()
{
	bIsPlayingJellyEffect = false;

	if (GetJellySceneComponent() == false)
	{
		return;
	}

	GetJellySceneComponent()->SetRelativeLocation(StartMeshLocation);
	GetJellySceneComponent()->SetRelativeRotation(StartMeshRotation);
	GetJellySceneComponent()->SetRelativeScale3D(StartMeshScale);
}

void UUSAJellyEffectComponent::TickJellyEffect()
{
	// 플레이 중인지 판단
	if (bIsPlayingJellyEffect)
	{
		// 현재 젤리 이펙트 스케일 초기화
		CurrentJellyEffectScale = FVector::OneVector;

		// 만약 데이터가 없으면 초기화 후 수행하지 않음
		if (IsValid(CurrentJellyEffectData) == false)
		{
			bIsPlayingJellyEffect = false;

			return;
		}

		// 만약 시간이 모두 경과하는 경우
		float CurrentJellyEffectTime = GetWorld()->GetTimeSeconds();
		if (CurrentJellyEffectTime > EndJellyEffectTime)
		{
			// 마지막 순간의 절리 이펙트 수행 (1.0f)
			CurrentJellyEffectLocation
				= CurrentJellyEffectData->GetLocationVectorByRatio(1.0f);
			CurrentJellyEffectRotation
				= FRotator::MakeFromEuler
				(CurrentJellyEffectData->GetRotationVectorByRatio(1.0f));
			CurrentJellyEffectScale
				= CurrentJellyEffectData->GetScaleVectorByRatio(1.0f);

			// 마지막 이펙트를 유지하지 않으면
			// bIsPlayingJellyEffect를 false로 설정하여 Tick 연산 멈추기
			if (CurrentJellyEffectData->GetJellyKeepLastEffect() == false)
			{
				bIsPlayingJellyEffect = false;
			}

			return;
		}

		// 현재 시간에 따라, 진행 정도 비율 계산
		float CurrentJellyEffectRatio 
			= (CurrentJellyEffectTime - PlayJellyEffectTime) 
			/ (EndJellyEffectTime - PlayJellyEffectTime);

		// 젤리 이펙트 수행
		CurrentJellyEffectLocation 
			= CurrentJellyEffectData->GetLocationVectorByRatio(CurrentJellyEffectRatio);
		CurrentJellyEffectRotation 
			= FRotator::MakeFromEuler
			(CurrentJellyEffectData->GetRotationVectorByRatio(CurrentJellyEffectRatio));
		CurrentJellyEffectScale 
			= CurrentJellyEffectData->GetScaleVectorByRatio(CurrentJellyEffectRatio);
	}
}

void UUSAJellyEffectComponent::TickJellyEffectByGravity()
{
	if (bIsUsingJellyEffectByGravity == false)
	{
		CurrentJellyEffectGravityScale = FVector::OneVector;

		return;
	}

	if (GetJellySceneComponent() == nullptr)
	{
		return;
	}

	ACharacter* Character = Cast <ACharacter>(GetOwner());
	if (Character == nullptr
		|| Character->GetCharacterMovement() == nullptr)
	{
		return;
	}

	if (ScaleByGravityRatio == nullptr)
	{
		return;
	}

	float CurrentRatio = Character->GetCharacterMovement()->Velocity.Z / MaxGravityForScale;
	CurrentRatio = FMath::Abs(CurrentRatio);
	CurrentRatio = FMath::Clamp(CurrentRatio, 0.0f, 1.0f);
	
	CurrentJellyEffectGravityScale = ScaleByGravityRatio->GetVectorValue(CurrentRatio);
}

void UUSAJellyEffectComponent::TickJellyEffectFinal()
{
	if (GetJellySceneComponent() == false)
	{
		return;
	}

	GetJellySceneComponent()->SetRelativeLocation(StartMeshLocation + CurrentJellyEffectLocation + CurrentJellyEffectCapsuleOffsetLocation);
	GetJellySceneComponent()->SetRelativeRotation(StartMeshRotation + CurrentJellyEffectRotation);
	GetJellySceneComponent()->SetRelativeScale3D((StartMeshScale * CurrentJellyEffectScale) * CurrentJellyEffectGravityScale);

	CurrentJellyEffectLocation = FVector::ZeroVector;
	CurrentJellyEffectRotation = FRotator::ZeroRotator;
	CurrentJellyEffectScale = FVector::OneVector;
}
