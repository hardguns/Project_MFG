// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MFG_HUDObjectivesContainer.generated.h"

class AMFG_GameMode;
class UMFG_HUDObjective;
class UVerticalBox;

UCLASS()
class PROJECT_MFG_API UMFG_HUDObjectivesContainer : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		AMFG_GameMode* GameModeReference;

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		TArray<UMFG_HUDObjective*> Objectives;

public:
	
	UFUNCTION(BlueprintCallable)
	void InitializeWidget();

	UFUNCTION(BlueprintCallable)
	void InitializeObjectives(TArray<UMFG_HUDObjective*> NewObjectives);

	UFUNCTION(BlueprintCallable)
	void CreateNewObjectives(TArray<UMFG_HUDObjective*> NewObjectives);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_CreateNewObjectives();

	UFUNCTION()
	void UpdateObjectives(FName IdentifierTag);
	
};
