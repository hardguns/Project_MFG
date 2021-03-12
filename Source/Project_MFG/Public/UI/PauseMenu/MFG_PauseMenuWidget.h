// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MFG_PauseMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MFG_API UMFG_PauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pause Menu")
		FName GameplayLevelName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pause Menu")
		FName MainMenuLevelName;

protected:

	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void ResumeGame();

	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void RestartGame();

	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void SaveGame();

	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void GoToMainMenu();

	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void QuitGame();

protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "Pause Menu")
	void BP_ResumeGame();

	UFUNCTION(BlueprintImplementableEvent, Category = "Pause Menu")
	void BP_RestartGame();

	UFUNCTION(BlueprintImplementableEvent, Category = "Pause Menu")
	void BP_SaveGame();

	UFUNCTION(BlueprintImplementableEvent, Category = "Pause Menu")
	void BP_GoToMainMenu();

	UFUNCTION(BlueprintImplementableEvent, Category = "Pause Menu")
	void BP_QuitGame();
	
};
