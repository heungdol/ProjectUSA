// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/USACharacterPlayer.h"

#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Camera/CameraComponent.h"
#include "Camera/USATargetingCameraActor.h"

#include "Kismet/GameplayStatics.h"

#include "Player/USAPlayerState.h"

#include "GAS/AttributeSet/USAAttributeSet.h"

#include "ProjectUSA.h"



AUSACharacterPlayer::AUSACharacterPlayer()
{
	//ASC = nullptr;
}

void AUSACharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	//if (GetPlayerState() != nullptr)
	//{
	//	if (GetPlayerState()->HasAuthority())
	//	{
	//		USA_LOG(LogTemp, Log, TEXT("Player State Has Authority"));
	//	}
	//	else
	//	{
	//		USA_LOG(LogTemp, Log, TEXT("Player State Has No Authority"));
	//	}
	//}
	//else
	//{
	//	USA_LOG(LogTemp, Log, TEXT("No Player State"));
	//}

	//if (GetController<APlayerController>() != nullptr)
	//{
	//	if (GetController<APlayerController>()->HasAuthority())
	//	{
	//		USA_LOG(LogTemp, Log, TEXT("Player Constroller Has Authority"));
	//	}
	//	else
	//	{
	//		USA_LOG(LogTemp, Log, TEXT("Player Constroller Has No Authority"));
	//	}
	//}
	//else
	//{
	//	USA_LOG(LogTemp, Log, TEXT("No Player Constroller"));
	//}

	InitPlayerController();
	InitTargetingCameraActor();
}

void AUSACharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsTargetingCamera != 0)
	{
		KeepTargeting();
	}
}


void AUSACharacterPlayer::Look(const FInputActionValue& Value)
{
	AUSAPlayerState* USAPlayerState = GetPlayerState <AUSAPlayerState>();

	if (USAPlayerState == nullptr)
	{
		Super::Look(Value);
		return;
	}
	
	float LookSensitivityRatio = USAPlayerState->GetLookSensitivityRatio();

	//USA_LOG(LogTemp, Log, TEXT("Current Look Sensitivity Ratio: %f"), LookSensitivityRatio);

	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X * LookSensitivityRatio);
		AddControllerPitchInput(LookAxisVector.Y * LookSensitivityRatio);
	}
}

void AUSACharacterPlayer::SetupGAS()
{
	AUSAPlayerState* USAPlayerState = GetPlayerState <AUSAPlayerState>();

	if (USAPlayerState != nullptr)
	{
		ASC = USAPlayerState->GetAbilitySystemComponent();

		if (ASC != nullptr)
		{
			ASC->InitAbilityActorInfo(USAPlayerState, this);
		}
	}

	Super::SetupGAS();
}

//


void AUSACharacterPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	//bool bIsMovable = false;
	//FRotator MoveRotation = FRotator::ZeroRotator;

	//if (bIsTargetingCamera != 0
	//	&& TargetingCameraActor != nullptr)
	//{
	//	bIsMovable = true;

	//	MoveRotation = TargetingCameraActor->GetActorRotation();
	//	MoveRotation.Pitch = 0.0f;
	//	MoveRotation.Roll = 0.0f;
	//}
	//else if (Controller != nullptr)
	//{
	//	bIsMovable = true;

	//	MoveRotation = Controller->GetControlRotation();
	//}

	//if (bIsMovable == false)
	//{
	//	return;
	//}

	if (PlayerController == nullptr)
	{
		return;
	}

	const FRotator MoveRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	const FRotator YawRotation(0, MoveRotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);

	USACharacterInputMovementDirection = ForwardDirection * MovementVector.Y;
	USACharacterInputMovementDirection += RightDirection * MovementVector.X;
}

void AUSACharacterPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 일반 클라에서 수행
	SetupGAS();
}

void AUSACharacterPlayer::OnRep_ASC()
{
	Super::OnRep_ASC();

	PostSetupGAS();

	SetupAttributeSet();

	BeginStartAbilities();

	K2_OnCurrentHealthRatioChanged(GetCharacterCurrentHealthRatio_Implementation());
}

void AUSACharacterPlayer::UpdateCurrentTargetableActor()
{
	// 오버랩으로 모든 액터 가져오기
	FVector SourceLocation = GetActorLocation();
	//float DistanceFromSourceToTarget = 0.0f;

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Visibility));
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	UClass* SeekClass = AActor::StaticClass();

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Init(this, 1);

	TArray<AActor*> TempTargetableActors_Overlap;

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), SourceLocation, TargetableActorRange, TraceObjectTypes, SeekClass, IgnoreActors, TempTargetableActors_Overlap);

	// 인터페이스를 이용하여 타깃 가능한 액터 가져오기
	TArray<TPair<float, AActor*>> TempTargetableActors_Scored;
	const float ScoreWeight = 0.6f;
	const float DotCutoff = 0.0f;

	for (AActor* TempActor : TempTargetableActors_Overlap)
	{
		if (TempActor->GetClass()->ImplementsInterface(UUSATargetableInterface::StaticClass()))
		{
			// 점수 계산

			// 1. 거리
			float DistanceFromSourceToTarget = (TempActor->GetActorLocation() - SourceLocation).Length();
			float CurrentTempActorScore_Distance = 1 - (DistanceFromSourceToTarget / TargetableActorRange);

			// 2. 방향 (내적)
			float CurrentTempActorScore_Direction = 0.1f;
			
			if (PlayerController != nullptr)
			{
				FVector DirectionFromSourceToTarget = TempActor->GetActorLocation() - SourceLocation;
				DirectionFromSourceToTarget.Normalize();

				FVector DirectionCamera = PlayerController->PlayerCameraManager->GetCameraRotation().Vector();
				CurrentTempActorScore_Direction = FVector::DotProduct(DirectionFromSourceToTarget, DirectionCamera);
			}

			if (CurrentTempActorScore_Direction < DotCutoff)
			{
				continue;
			}

			// ...
			float CurrentTempActorScore = (CurrentTempActorScore_Distance * ScoreWeight) + (CurrentTempActorScore_Direction * (1 - ScoreWeight));

			TempTargetableActors_Scored.Add({ CurrentTempActorScore, TempActor });
		}
	}

	TempTargetableActors_Scored.Sort();

	// 가장 가까운 액터를 타깃으로 
	if (TempTargetableActors_Scored.IsEmpty() == false)
	{
		CurrentTargetableActor = TempTargetableActors_Scored[TempTargetableActors_Scored.Num() - 1].Value;
	}
}

void AUSACharacterPlayer::UpdateCurrentTargetableActor_Instant()
{
	// 오버랩으로 모든 액터 가져오기
	FVector SourceLocation = GetActorLocation();
	//float DistanceFromSourceToTarget = 0.0f;

	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Visibility));
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));


	UClass* SeekClass = AActor::StaticClass();

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Init(this, 1);

	TArray<AActor*> TempTargetableActors_Overlap;

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), SourceLocation, TargetableActorRange, TraceObjectTypes, SeekClass, IgnoreActors, TempTargetableActors_Overlap);

	// 인터페이스를 이용하여 타깃 가능한 액터 가져오기
	TArray<TPair<float, AActor*>> TempTargetableActors_Scored;
	const float ScoreWeight = 0.6f;
	const float DotCutoff = 0.0f;

	for (AActor* TempActor : TempTargetableActors_Overlap)
	{
		if (TempActor->GetClass()->ImplementsInterface(UUSATargetableInterface::StaticClass()))
		{
			// 점수 계산

			// 1. 거리
			float DistanceFromSourceToTarget = (TempActor->GetActorLocation() - SourceLocation).Length();
			float CurrentTempActorScore_Distance = 1 - (DistanceFromSourceToTarget / TargetableActorRange);

			// 2. 방향 (내적)
			float CurrentTempActorScore_Direction = 0.1f;

			if (PlayerController != nullptr)
			{
				FVector DirectionFromSourceToTarget = TempActor->GetActorLocation() - SourceLocation;
				DirectionFromSourceToTarget.Normalize();

				// 임시와 포커스의 차이점
				FVector DirectionCamera = GetActorForwardVector();
				CurrentTempActorScore_Direction = FVector::DotProduct(DirectionFromSourceToTarget, DirectionCamera);
			}

			if (CurrentTempActorScore_Direction < DotCutoff)
			{
				continue;
			}

			// ...
			float CurrentTempActorScore = (CurrentTempActorScore_Distance * ScoreWeight) + (CurrentTempActorScore_Direction * (1 - ScoreWeight));

			TempTargetableActors_Scored.Add({ CurrentTempActorScore, TempActor });
		}
	}

	TempTargetableActors_Scored.Sort();

	// 가장 가까운 액터를 타깃으로 설정
	if (TempTargetableActors_Scored.IsEmpty() == false)
	{
		CurrentTargetableActor = TempTargetableActors_Scored[TempTargetableActors_Scored.Num() - 1].Value;
	}
}

void AUSACharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AUSACharacterPlayer::InitTargetingCameraActor()
{
	if (GetWorld() == nullptr)
	{
		return;
	}

	if (GetWorld()->GetNetDriver() == nullptr)
	{
		return;
	}

	if (GetWorld()->GetNetDriver()->IsServer() == true)
	{
		if (GetLocalRole() == ENetRole::ROLE_Authority
			&& GetRemoteRole() == ENetRole::ROLE_SimulatedProxy)
		{
			// 리슨 서버의 클라이언트
		}
		else
		{
			return;
		}
	}
	else
	{
		if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
		{
			// 클라이언트
		}
		else
		{
			return;
		}
	}

	TargetingCameraActor = GetWorld()->SpawnActor<AUSATargetingCameraActor>(TargetingCameraActorClass, GetActorTransform());
}


void AUSACharacterPlayer::InitPlayerController()
{
	PlayerController = GetController<APlayerController>();
}


void AUSACharacterPlayer::DoTarget(const struct FInputActionValue& Value)
{
	Super::DoTarget(Value);

	if (TargetingCameraActor == nullptr)
	{
		InitTargetingCameraActor();
	}

	if (TargetingCameraActor == nullptr)
	{
		return;
	}

	if (bIsTargetingCamera == false)
	{
		StartTargeting();
	}
	else
	{
		FinishTargeting();
	}
}

void AUSACharacterPlayer::StartTargeting()
{
	if (TargetingCameraActor == nullptr)
	{
		return;
	}

	UpdateCurrentTargetableActor();

	if (CurrentTargetableActor == nullptr)
	{
		return;
	}

	TargetingCameraActor->SetSourceActor(this);
	TargetingCameraActor->SetTargetActor(CurrentTargetableActor);

	TargetingCameraActor->SetOnOff(true);
	bIsTargetingCamera = true;

	K2_OnStartTargeting();
}

void AUSACharacterPlayer::KeepTargeting()
{
	if (TargetingCameraActor == nullptr)
	{
		return;
	}

	if (PlayerController
		&& TargetingCameraActor != nullptr)
	{
		PlayerController->SetControlRotation(TargetingCameraActor->GetActorRotation());
	}

	if (CurrentTargetableActor == nullptr)
	{
		FinishTargeting();
		return;
	}

	K2_OnKeepTargeting();
}

void AUSACharacterPlayer::ChangeTargeting()
{
	// ...
}

void AUSACharacterPlayer::FinishTargeting()
{
	if (TargetingCameraActor == nullptr)
	{
		return;
	}

	TargetingCameraActor->SetOnOff(false);
	bIsTargetingCamera = false;

	TargetingCameraActor->SetTargetActor(nullptr);
	SetCurrentTargetableActorNullptr();

	if (CameraSpringArmComponent != nullptr
		&& TargetingCameraActor != nullptr)
	{
		CameraSpringArmComponent->SetWorldRotation(TargetingCameraActor->GetActorRotation());
	}

	K2_OnFinishTargeting();
}
