// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "MFG_MainMenuButton.generated.h"

UENUM()
enum class EMFG_ButtonStyleType : uint8
{
	ButtonStyleType_Mouse			UMETA(DisplayName = "Selected by mouse"),
	ButtonStyleType_KeySelected		UMETA(DisplayName = "Selected by keyboard"),
	ButtonStyleType_KeyNotSelected	UMETA(DisplayName = "Not Selected by keyboard")
};

/**
 * 
 */

UCLASS()
class PROJECT_MFG_API UMFG_MainMenuButton : public UButton
{
	GENERATED_BODY()

public:

	UMFG_MainMenuButton();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MainMenuButton")
	float CheckFocusRate;

	FTimerHandle TimerHandle_CheckFocus;
	
protected:

	UFUNCTION(BlueprintCallable, Category = "Main Menu Button")
	void Start();

	UFUNCTION(BlueprintCallable, Category = "Button")
	void CheckFocus();

	void SetButtonStyle(EMFG_ButtonStyleType NewStyleType);

	UFUNCTION(BlueprintImplementableEvent, Category = "Main Menu Button")
	void BP_SetButtonStyle(EMFG_ButtonStyleType NewStyleType);
};
