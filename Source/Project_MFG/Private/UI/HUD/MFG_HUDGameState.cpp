// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/MFG_HUDGameState.h"
#include "Core/MFG_GameMode.h"

void UMFG_HUDGameState::InitializeWidget()
{
	GameModeReference = Cast<AMFG_GameMode>(GetWorld()->GetAuthGameMode());
	if (IsValid(GameModeReference))
	{
		GameModeReference->OnVictoryDelegate.AddDynamic(this, &UMFG_HUDGameState::OnVictory);
		GameModeReference->OnGameOverDelegate.AddDynamic(this, &UMFG_HUDGameState::OnGameOver);
	}

	SetVisibility(ESlateVisibility::Hidden);
}

void UMFG_HUDGameState::OnVictory()
{
	GameStateName = VictoryName;
	SetVisibility(ESlateVisibility::Visible);
}

void UMFG_HUDGameState::OnGameOver()
{
	GameStateName = GameOverName;
	SetVisibility(ESlateVisibility::Visible);
}
