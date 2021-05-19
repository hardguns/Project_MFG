// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MFG_GameMode.h"
#include "MFG_Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MFG_SpectatingCamera.h"
#include "Sound/SoundCue.h"
#include "Enemy/MFG_Enemy.h"

AMFG_GameMode::AMFG_GameMode()
{
	MainMenuMapName = "MainMenuMap";
}

void AMFG_GameMode::BeginPlay()
{
	Super::BeginPlay();

	SetupSpectatingCameras();

	TArray<AActor*> EnemyActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMFG_Enemy::StaticClass(), EnemyActors);

	for (AActor* EnemyActor : EnemyActors)
	{
		if (!IsValid(EnemyActor))
		{
			continue;
		}

		AMFG_Enemy* NewEnemy = Cast<AMFG_Enemy>(EnemyActor);

		if (IsValid(NewEnemy))
		{
			LevelEnemies.AddUnique(NewEnemy);
		}
	}

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

void AMFG_GameMode::PlayMusic(USoundCue* MusicCue)
{
	if (!IsValid(MusicCue))
	{
		return;
	}

	UGameplayStatics::PlaySound2D(GetWorld(), MusicCue);
}

void AMFG_GameMode::AddKeyToCharacter(AMFG_Character* KeyOwner, FName KeyTag)
{
	if(IsValid(KeyOwner))
	{
		OnKeyAddedDelegate.Broadcast(KeyTag);
		KeyOwner->AddKey(KeyTag);
	}
}

void AMFG_GameMode::EnableElectricity(FName ObjectiveIdentier)
{
	OnEnergyEnabledDelegate.Broadcast(ObjectiveIdentier);
}

void AMFG_GameMode::Victory(AMFG_Character* Character)
{
	Character->DisableInput(nullptr);

	MoveCameraToSpectatingPoint(Character, VictoryCamera);
	OnVictoryDelegate.Broadcast();

	PlayMusic(VictoryMusic);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_BackToMainMenu, this, &AMFG_GameMode::BackToMainMenu, 4.0f, false);
	
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

	PlayMusic(GameOverMusic);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_BackToMainMenu, this, &AMFG_GameMode::BackToMainMenu, 8.0f, false);

	BP_GameOver(Character);
}

void AMFG_GameMode::BackToMainMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), MainMenuMapName);
}

void AMFG_GameMode::CheckAlertMode()
{
	bool bEnemyInAlertMode = false;

	for (AMFG_Enemy* EnemyOnLevel : LevelEnemies)
	{
		if (!IsValid(EnemyOnLevel))
		{
			continue;
		}

		if (EnemyOnLevel->IsAlert())
		{
			bEnemyInAlertMode = true;
			break;
		}
	}

	if (bIsAlertMode != bEnemyInAlertMode)
	{
		bIsAlertMode = bEnemyInAlertMode;
		OnAlertModeChangeDelegate.Broadcast(bIsAlertMode);
	}
}

void AMFG_GameMode::AddEnemyToLevel(AMFG_Enemy* NewEnemy)
{
	LevelEnemies.Add(NewEnemy);
}
