// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MFG_HUDServerHacking.generated.h"

class AMFG_HackedServer;

/**
 * 
 */
UCLASS()
class PROJECT_MFG_API UMFG_HUDServerHacking : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		bool bIsActive;
	
	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		float CurrentHackingTime;

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		float TotalHackingTime;

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		float HackingPercentage;

	FTimerHandle TimerHandle_HackingTime;

public: 

	UFUNCTION(BlueprintCallable)
	void InitializeWidget();

	void ShowWidget();

	void HideWidget();

	UFUNCTION()
	void StartHacking(float HackingTime);

	void FillBar();
	
};
