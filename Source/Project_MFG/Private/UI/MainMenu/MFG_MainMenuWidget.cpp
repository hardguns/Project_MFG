// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/MFG_MainMenuWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Core/MFG_GameInstance.h"

void UMFG_MainMenuWidget::NewGame()
{
	BP_NewGame();

	UMFG_GameInstance* GameInstanceReference = Cast<UMFG_GameInstance>(GetWorld()->GetGameInstance());
	if (IsValid(GameInstanceReference))
	{
		GameInstanceReference->ResetData();
	}

	UGameplayStatics::OpenLevel(GetWorld(), GameplayLevelName);
}

void UMFG_MainMenuWidget::ContinueGame()
{
	BP_ContinueGame();

	UMFG_GameInstance* GameInstanceReference = Cast<UMFG_GameInstance>(GetWorld()->GetGameInstance());
	if (IsValid(GameInstanceReference))
	{
		GameInstanceReference->LoadData();
	}

	UGameplayStatics::OpenLevel(GetWorld(), GameplayLevelName);
}

void UMFG_MainMenuWidget::QuitGame()
{
	BP_QuitGame();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
}
