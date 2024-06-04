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
#include "Camera/USAPlacedCameraActor.h"

#include "Kismet/GameplayStatics.h"

#include "Player/USAPlayerState.h"

#include "GAS/AttributeSet/USAAttributeSet.h"

#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"

#include "Item/USAItemBase.h"

#include "ProjectUSA.h"


AUSACharacterPlayer::AUSACharacterPlayer()
{
	//ASC = nullptr;
}

void AUSACharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	InitPlayerController();
	InitTargetingCameraActor();

	K2_OnCurrentHealthRatioChanged(GetCharacterCurrentHealthRatio_Implementation());
}

void AUSACharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (IsValid(CurrentTargetableActor) == true)
	{
		KeepTargeting();
	}

	//if (IsValid(PlayerController) && IsValid(PlayerController->PlayerCameraManager))
	//{
	//	USA_LOG(LogTemp, Log, TEXT("Capsule Yaw: %f, Mesh Yaw: %f,Camera Yaw: %f"),
	//		GetCapsuleComponent()->GetComponentRotation().Yaw,
	//		GetMesh()->GetComponentRotation().Yaw,
	//		PlayerController->PlayerCameraManager->GetCameraRotation().Yaw);
	//}

}

//

void AUSACharacterPlayer::InitPlayerController()
{
	PlayerController = GetController<APlayerController>();
}

void AUSACharacterPlayer::InitTargetingCameraActor()
{
	if (PlayerController == nullptr)
	{
		return;
	}

	if (PlayerController->IsLocalController() == false)
	{
		return;
	}

	TargetingCameraActor = GetWorld()->SpawnActor<AUSATargetingCameraActor>(TargetingCameraActorClass, GetActorTransform());

	if (IsValid(TargetingCameraActor) == true)
	{
		TargetingCameraActor->SetSourceActor(this);
	}
}

//

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


void AUSACharacterPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (PlayerController == nullptr
		|| PlayerController->PlayerCameraManager == nullptr)
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

//

void AUSACharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
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

	CheckCharacterByGameplayTags();
}

//

//void AUSACharacterPlayer::DoDrop(const FInputActionValue& Value)
//{
//	Super::DoDrop(Value);
//
//	DropWeapons();
//}

void AUSACharacterPlayer::OnPickableDetectBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UKismetSystemLibrary::IsServer(GetWorld()) == false
		&& UKismetSystemLibrary::IsStandalone(GetWorld()) == false)
	{
		return;
	}

	if (bIsPickable == false)
	{
		return;
	}

	IUSAPickableInterface* PickableInterface = Cast<IUSAPickableInterface>(OtherActor);
	if (PickableInterface != nullptr)
	{
		PickUpSomething(PickableInterface);
	}
}

//

void AUSACharacterPlayer::LookTarget(const struct FInputActionValue& Value)
{
	Super::LookTarget(Value);

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
		UpdateCurrentTargetableActor();

		StartTargeting();
	}
	else
	{
		FinishTargeting();
	}
}

//

void AUSACharacterPlayer::UpdateCurrentTargetableActor()
{
	if (PlayerController == nullptr
		|| PlayerController->PlayerCameraManager == nullptr)
	{
		return;
	}

	// 카메라 방향 기준으로 타깃 설정
	SetCurrentTargetableActorUsingForwardVector(PlayerController->PlayerCameraManager->GetCameraRotation().Vector(), CurrentTargetableActor);
}

void AUSACharacterPlayer::UpdateCurrentTargetableActor_Instant()
{
	//if (GetCharacterMovement() == nullptr)
	//{
	//	return;
	//}

	// 플레이어 방향 기준으로 타깃 설정
	//SetCurrentTargetableActorUsingForwardVector(GetUSACharacterDirection_InputMovement(), CurrentTargetableActor_Instant);

	if (PlayerController == nullptr
		|| PlayerController->PlayerCameraManager == nullptr)
	{
		return;
	}

	// 카메라 방향 기준으로 타깃 설정
	SetCurrentTargetableActorUsingForwardVector(PlayerController->PlayerCameraManager->GetCameraRotation().Vector(), CurrentTargetableActor_Instant);
}

void AUSACharacterPlayer::SetCurrentTargetableActorUsingForwardVector(const FVector& InDirection, TObjectPtr<class AActor>& InOutTargetActorPointer)
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
	const float DotCutoff = 0.1f;

	for (AActor* TempActor : TempTargetableActors_Overlap)
	{
		if (IsValid(TempActor) == false)
		{
			continue;
		}

		if (Cast<IUSATargetableInterface>(TempActor) == nullptr)
		{
			continue;
		}

		//if (TempActor->GetClass()->ImplementsInterface(UUSATargetableInterface::StaticClass()))
		//{
			// 만약 타겟팅이 불가능 하면 무시
			if (Cast<IUSATargetableInterface>(TempActor)->GetIsTargetableCurrently() == false)
			{
				continue;
			}

			// 점수 계산

			// 1. 거리
			float DistanceFromSourceToTarget = (TempActor->GetActorLocation() - SourceLocation).SquaredLength();
			float CurrentTempActorScore_Distance = 1 - (DistanceFromSourceToTarget / (TargetableActorRange * TargetableActorRange));

			// 2. 방향 (내적)
			float CurrentTempActorScore_Direction = 0.1f;
	
			FVector DirectionFromSourceToTarget = TempActor->GetActorLocation() - SourceLocation;
			DirectionFromSourceToTarget.Normalize();

			// 만약 PlaceCamera가 작동 중이라면 캐릭터의 방향으로 판단
			if (IsValid(PlacedCameraActor) == true)
			{
				CurrentTempActorScore_Direction = FVector::DotProduct(DirectionFromSourceToTarget, GetActorForwardVector());
			}
			else
			{
				// 임시와 포커스의 차이점
				CurrentTempActorScore_Direction = FVector::DotProduct(DirectionFromSourceToTarget, InDirection);
			}

			// 만약 정 반대의 방향을 가리키면 무시
			if (CurrentTempActorScore_Direction < DotCutoff)
			{
				continue;
			}

			// ...
			float CurrentTempActorScore = (CurrentTempActorScore_Distance * ScoreWeight) + (CurrentTempActorScore_Direction * (1 - ScoreWeight));

			TempTargetableActors_Scored.Add({ CurrentTempActorScore, TempActor });
		//}
	}

	TempTargetableActors_Scored.Sort();

	// 가장 가까운 액터를 타깃으로 설정
	if (TempTargetableActors_Scored.IsEmpty() == false)
	{
		InOutTargetActorPointer = TempTargetableActors_Scored[TempTargetableActors_Scored.Num() - 1].Value;
	}
	else
	{
		InOutTargetActorPointer = nullptr;
	}
}

//

void AUSACharacterPlayer::StartTargeting()
{
	if (TargetingCameraActor == nullptr)
	{
		return;
	}

	if (CurrentTargetableActor == nullptr)
	{
		FinishTargeting();
		return;
	}

	TargetingCameraActor->SetSourceActor(this);
	TargetingCameraActor->SetTargetActor(CurrentTargetableActor);

	//TargetingCameraActor->SetOnOff(true);
	bIsTargetingCamera = true;

	ManageAllCamera();

	//K2_OnStartTargeting();
}

void AUSACharacterPlayer::KeepTargeting()
{
	if (TargetingCameraActor == nullptr)
	{
		FinishTargeting();
		return;
	}

	if (CurrentTargetableActor == nullptr)
	{
		FinishTargeting();
		return;
	}


	IUSATargetableInterface* Targetable = Cast<IUSATargetableInterface>(CurrentTargetableActor);
	if (Targetable == nullptr)
	{
		ChangeTargeting();
		return;
	}

	if (Targetable != nullptr && Targetable->GetIsTargetableCurrently() == false)
	{
		ChangeTargeting();
		return;
	}

	//if (PlayerController
	//	&& TargetingCameraActor != nullptr)
	//{
	//	PlayerController->SetControlRotation(TargetingCameraActor->GetActorRotation());
	//}

	FVector SourceLocation = GetActorLocation();
	FVector TargetLocation = CurrentTargetableActor->GetActorLocation();

	if ((SourceLocation-TargetLocation).SquaredLength() > TargetableActorRange * TargetableActorRange)
	{
		FinishTargeting();
		return;
	}

	//if (IsValid(PlacedCameraActor) == false)
	//{
	//	K2_OnKeepTargeting();
	//}
}

void AUSACharacterPlayer::ChangeTargeting()
{
	if (TargetingCameraActor == nullptr)
	{
		return;
	}

	UpdateCurrentTargetableActor();

	if (CurrentTargetableActor == nullptr)
	{
		FinishTargeting();
		return;
	}

	TargetingCameraActor->SetSourceActor(this);
	TargetingCameraActor->SetTargetActor(CurrentTargetableActor);
}

void AUSACharacterPlayer::FinishTargeting()
{
	if (TargetingCameraActor == nullptr)
	{
		return;
	}

	TargetingCameraActor->SetSourceActor(this);
	TargetingCameraActor->SetTargetActor(nullptr);

	//if (PlayerController
//	&& TargetingCameraActor != nullptr)
//{
//	PlayerController->SetControlRotation(TargetingCameraActor->GetActorRotation());
//}

	if (PlayerController != nullptr
		&& CameraSpringArmComponent != nullptr
		&& TargetingCameraActor != nullptr
		&& bIsTargetingCamera != false)
	{
		PlayerController->SetControlRotation(TargetingCameraActor->GetActorRotation());
		CameraSpringArmComponent->SetWorldRotation(TargetingCameraActor->GetActorRotation());
		CameraSpringArmComponent->UpdateChildTransforms();
	}

	//TargetingCameraActor->SetOnOff(false);
	bIsTargetingCamera = false;

	TargetingCameraActor->SetTargetActor(nullptr);
	CurrentTargetableActor = nullptr;


	ManageAllCamera();

	//K2_OnFinishTargeting();
}

//

void AUSACharacterPlayer::StartPlacedCamera(AUSAPlacedCameraActor* InActor)
{
	if (IsValid(PlayerController) == false)
	{
		return;
	}

	if (PlayerController->IsLocalPlayerController() == false)
	{
		return;
	}

	PlacedCameraActor = InActor;

	USA_LOG(LogTemp, Log, TEXT("Rotation IN: %s"), *CameraSpringArmComponent->GetComponentRotation().ToString());

	if (IsValid(PlacedCameraActor) == true)
	{
		FVector ActorForward = GetActorForwardVector();
		FVector CameraForward = GetActorForwardVector();

		if (IsValid(PlayerController) == true
			&& IsValid(PlayerController->PlayerCameraManager) == true)
		{
			CameraForward = PlayerController->PlayerCameraManager->GetCameraRotation().Vector();
		}

		PlacedCameraActor->InitPlacedCameraActor(ActorForward, CameraForward);
	}

	ManageAllCamera();

	//K2_OnStartPlacedCamera();
}

void AUSACharacterPlayer::FinishPlacedCamera(AUSAPlacedCameraActor* InActor)
{
	if (IsValid(PlayerController) == false)
	{
		return;
	}

	if (PlayerController->IsLocalPlayerController() == false)
	{
		return;
	}

	if (PlacedCameraActor != InActor)
	{
		return;
	}

	USA_LOG(LogTemp, Log, TEXT("Rotation From: %s"), *CameraSpringArmComponent->GetComponentRotation().ToString());

	if (PlayerController != nullptr
		&& CameraSpringArmComponent != nullptr
		&& PlacedCameraActor != nullptr)
	{
		PlayerController->SetControlRotation(PlacedCameraActor->GetActiveCameraRotation());
		CameraSpringArmComponent->SetWorldRotation(PlacedCameraActor->GetActiveCameraRotation());
		CameraSpringArmComponent->UpdateChildTransforms();
	}

	USA_LOG(LogTemp, Log, TEXT("Rotation To: %s"), *CameraSpringArmComponent->GetComponentRotation().ToString());

	PlacedCameraActor = nullptr;

	//ManageAllCame(ra();

	StartTargeting();
}

//

void AUSACharacterPlayer::ManageAllCamera()
{
	if (IsValid(PlayerController) == false)
	{
		return;
	}

	if (PlayerController->IsLocalPlayerController() == false)
	{
		return;
	}

	if (IsValid(PlacedCameraActor) == false)
	{
		if (CurrentTargetableActor)
		{
			K2_OnStartTargeting();
		}
		else
		{
			K2_OnFinishTargeting();
		}
	}
	else
	{
		K2_OnStartPlacedCamera();
	}
}

//

void AUSACharacterPlayer::StartCameraShake_HitSuccess(TSubclassOf<class UDamageType> DamageType)
{
	Super::StartCameraShake_HitSuccess(DamageType);

	if (HitSuccessCameraShakes.Contains(DamageType) == false)
	{
		return;
	}

	if (PlayerController == nullptr
		|| PlayerController->PlayerCameraManager == nullptr)
	{
		return;
	}

	PlayerController->PlayerCameraManager->StartCameraShake(HitSuccessCameraShakes[DamageType]);
}