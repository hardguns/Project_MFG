// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MFG_HUDCharacterImage.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MFG_API UMFG_HUDCharacterImage : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		UTexture2D* CharacterIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
		float StartDelayDuration;

	FTimerHandle TimerHandle_LoadWidget;

protected:

	UFUNCTION(BlueprintCallable)
	void Start();

	UFUNCTION(BlueprintCallable)
	void InitializeWidget();
	
};
