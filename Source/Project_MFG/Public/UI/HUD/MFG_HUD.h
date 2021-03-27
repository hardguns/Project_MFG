// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MFG_HUD.generated.h"

class UMFG_HUDRadar;
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
	
};
