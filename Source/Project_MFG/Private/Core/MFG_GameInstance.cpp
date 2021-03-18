// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MFG_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SaveSystem/MFG_SaveGame.h"

UMFG_GameInstance::UMFG_GameInstance()
{
	SaveSlotName = "MFG_SaveData";
}

void UMFG_GameInstance::AddEnemyDefeatedToCounter()
{
	EnemiesDefeatedCounter++;
	OnEnemyKilledDelegate.Broadcast(EnemiesDefeatedCounter);
	BP_AddEnemyDefeatedToCounter();
}

void UMFG_GameInstance::SaveData()
{
	USaveGame* SaveGameObject = nullptr;
	bool bExistingData = UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0);
	if (bExistingData)
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(UMFG_SaveGame::StaticClass());
	}

	if (IsValid(SaveGameObject))
	{
		UMFG_SaveGame* SaveFile = Cast<UMFG_SaveGame>(SaveGameObject);
		if (IsValid(SaveFile))
		{
			SaveFile->SetEnemiesDefeatedCounterInfo(EnemiesDefeatedCounter);

			UGameplayStatics::SaveGameToSlot(SaveFile, SaveSlotName, 0);
		}
	}
}

void UMFG_GameInstance::LoadData()
{
	USaveGame* SaveGameObject = nullptr;
	bool bExistingData = UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0);
	if (bExistingData)
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0);
	}

	if (IsValid(SaveGameObject))
	{
		UMFG_SaveGame* SaveFile = Cast<UMFG_SaveGame>(SaveGameObject);
		if (IsValid(SaveFile))
		{
			EnemiesDefeatedCounter = SaveFile->GetEnemiesDefeatedCounterInfo();
		}
	}
}

void UMFG_GameInstance::ResetData()
{
	EnemiesDefeatedCounter = 0;
	BP_ResetData();
}
