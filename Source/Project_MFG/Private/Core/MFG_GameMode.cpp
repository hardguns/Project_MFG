// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MFG_GameMode.h"
#include "MFG_Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MFG_SpectatingCamera.h"

AMFG_GameMode::AMFG_GameMode()
{
	MainMenuMapName = "MainMenuMap";
}

void AMFG_GameMode::BeginPlay()
{
	Super::BeginPlay();

	SetupSpectatingCameras();

}

void AMFG_GameMode::SetupSpectatingCameras()
{
	TArray<AActor*> SpectatingCameraActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMFG_SpectatingCamera::StaticClass(), SpectatingCameraActors);

	if (SpectatingCameraActors.Num() > 0)
	{
		for (AActor* SpectatingActor : SpectatingCameraActors)
		{
			AMFG_SpectatingCamera* SpectatingCamera = Cast<AMFG_SpectatingCamera>(SpectatingActor);

			if (IsValid(SpectatingCamera))
			{
				switch (SpectatingCamera->GetCameraType())
				{
				case EMFG_SpectatingCameraType::CameraType_Victory:
					VictoryCamera = SpectatingCamera;
					break;
				case EMFG_SpectatingCameraType::CameraType_GameOver:
					GameOverCamera = SpectatingCamera;
					break;
				default:
					break;
				}
			}
		}
	}
}

void AMFG_GameMode::MoveCameraToSpectatingPoint(AMFG_Character* Character, AMFG_SpectatingCamera* SpectatingCamera)
{
	if (!IsValid(Character) && !IsValid(SpectatingCamera))
	{
		return;
	}

	AController* CharacterController = Character->GetController();
	if (IsValid(CharacterController))
	{
		APlayerController* PlayerController = Cast<APlayerController>(CharacterController);
		if (IsValid(PlayerController))
		{
			PlayerController->SetViewTargetWithBlend(SpectatingCamera, SpectatingBlendTime, EViewTargetBlendFunction::VTBlend_Cubic);
		}
	}
}

void AMFG_GameMode::AddKeyToCharacter(AMFG_Character* KeyOwner, FName KeyTag)
{
	if(IsValid(KeyOwner))
	{
		OnKeyAddedDelegate.Broadcast(KeyTag);
		KeyOwner->AddKey(KeyTag);
	}
}

void AMFG_GameMode::Victory(AMFG_Character* Character)
{
	Character->DisableInput(nullptr);

	MoveCameraToSpectatingPoint(Character, VictoryCamera);
	OnVictoryDelegate.Broadcast();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_BackToMainMenu, this, &AMFG_GameMode::BackToMainMenu, 3.0f, false);
	
	BP_Victory(Character);
}

void AMFG_GameMode::GameOver(AMFG_Character* Character)
{
	Character->GetMovementComponent()->StopMovementImmediately();
	Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (Character->HasToDestroy())
	{
		//Used in aerial camera functions... detach pawn from controller
		Character->DetachFromControllerPendingDestroy();
		Character->SetLifeSpan(5.0f);
	}
	else 
	{
		Character->DisableInput(nullptr);
		MoveCameraToSpectatingPoint(Character, GameOverCamera);
	}

	OnGameOverDelegate.Broadcast();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_BackToMainMenu, this, &AMFG_GameMode::BackToMainMenu, 3.0f, false);

	BP_GameOver(Character);
}

void AMFG_GameMode::BackToMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), MainMenuMapName);
}
