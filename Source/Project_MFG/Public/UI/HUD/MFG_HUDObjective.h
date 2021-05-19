// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MFG_HUDObjective.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MFG_API UMFG_HUDObjective : public UUserWidget
{
	GENERATED_BODY()

public:
		
	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		bool bIsCompleted;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup", meta = (ExposeOnSpawn = "true"))
		FName ObjectiveName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
		FName ObjectiveIdentifier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
		FLinearColor CurrentColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
		FLinearColor CompletedColor;

public:

	void ObjectiveCompleted();

	FName GetObjectiveIdentifier() { return ObjectiveIdentifier; };
	
};
