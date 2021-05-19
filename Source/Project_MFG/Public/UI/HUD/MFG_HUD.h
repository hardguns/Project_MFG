// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MFG_HUD.generated.h"

class UMFG_HUDRadar;
class UMFG_HUDObjectivesContainer;
/**
 * 
 */
UCLASS()
class PROJECT_MFG_API UMFG_HUD : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "Setup", meta = (BindWidget))
		UMFG_HUDRadar* W_Radar;

	UPROPERTY(BlueprintReadOnly, Category = "Setup", meta = (BindWidget))
		UMFG_HUDObjectivesContainer* W_ObjectivesContainer;

public:

	UMFG_HUDObjectivesContainer* GetObjectivesContainer() { return W_ObjectivesContainer; };

};
