// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/MFG_HUDEnemyCounter.h"
#include "Core/MFG_GameInstance.h"

void UMFG_HUDEnemyCounter::InitializeWidget()
{
	GameInstanceReference = Cast<UMFG_GameInstance>(GetWorld()->GetGameInstance());

	if (IsValid(GameInstanceReference))
	{
		GameInstanceReference->OnEnemyKilledDelegate.AddDynamic(this, &UMFG_HUDEnemyCounter::UpdateCounter);
		UpdateCounter(GameInstanceReference->GetEnemiesDefeatedCounter());
	}
}

void UMFG_HUDEnemyCounter::UpdateCounter(int EnemyDefeatedCounter)
{
	EnemiesDefeated = EnemyDefeatedCounter;
}
