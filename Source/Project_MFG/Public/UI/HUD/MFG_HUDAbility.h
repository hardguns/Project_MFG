// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MFG_HUDAbility.generated.h"

class AMFG_Ability;
class UProgressBar;
/**
 * 
 */
UCLASS()
class PROJECT_MFG_API UMFG_HUDAbility : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
		float StartDelayDuration;

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
		UTexture2D* AbilityIcon;

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		TArray<AMFG_Ability*> CharacterAbilities;

	FTimerHandle TimerHandle_ReloadAbility;

	FTimerHandle TimerHandle_LoadWidget;

public:
	
	UFUNCTION(BlueprintCallable)
	void Start();
	
	UFUNCTION(BlueprintCallable)
	void InitializeWidget();

	UFUNCTION()
	void UpdateAbilityState(int AbilityAmountAvailable, int ReceivedAbilityIndex, bool bShowReloadingBar);
		
	void ReloadBar();
	
};
