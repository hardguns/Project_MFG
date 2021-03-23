// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MFG_Character.h"
#include "MFG_HUDAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MFG_API UMFG_HUDAbility : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		float AbilityPercentage;

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		float AbilityTotalCoolDown;

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		float AbilityCurrentCoolDown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
		float ReloadingBarSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		int MaxAbilityAmount;

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		int CurrentAbilityAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
		int AbilityIndex;

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		UTexture* AbilityIcon;

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		TArray<FAbility> CharacterAbilities;

	FTimerHandle TimerHandle_ReloadAbility;

public:

	UFUNCTION(BlueprintCallable)
	void InitializeWidget();

	UFUNCTION()
	void UpdateAbilityState(int AbilityAmountAvailable, int ReceivedAbilityIndex);
		
	void ReloadBar();
	
};
