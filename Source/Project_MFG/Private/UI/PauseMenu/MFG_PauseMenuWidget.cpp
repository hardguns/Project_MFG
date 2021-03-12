// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PauseMenu/MFG_PauseMenuWidget.h"
#include "Core/MFG_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Core/MFG_PlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UMFG_PauseMenuWidget::ResumeGame()
{
	BP_ResumeGame();

	AMFG_PlayerController* PlayerController =  Cast<AMFG_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);

	PlayerController->bShowMouseCursor = false;

	UGameplayStatics::SetGamePaused(GetWorld(), false);

	RemoveFromParent();
}

void UMFG_PauseMenuWidget::RestartGame()
{
	BP_RestartGame();

	UMFG_GameInstance* GameInstanceReference = Cast<UMFG_GameInstance>(GetWorld()->GetGameInstance());
	if (IsValid(GameInstanceReference))
	{
		GameInstanceReference->ResetData();
	}

	UGameplayStatics::OpenLevel(GetWorld(), GameplayLevelName);

	RemoveFromParent();
}

void UMFG_PauseMenuWidget::SaveGame()
{
	BP_SaveGame();

	UMFG_GameInstance* GameInstanceReference = Cast<UMFG_GameInstance>(GetWorld()->GetGameInstance());
	if (IsValid(GameInstanceReference))
	{
		GameInstanceReference->SaveData();
	}
}

void UMFG_PauseMenuWidget::GoToMainMenu()
{
	BP_GoToMainMenu();

	UGameplayStatics::OpenLevel(GetWorld(), MainMenuLevelName);

	RemoveFromParent();
}

void UMFG_PauseMenuWidget::QuitGame()
{
	BP_QuitGame();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
}
