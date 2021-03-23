// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MFG_HUDHealthBar.generated.h"

class UWidgetAnimation;

UCLASS()
class PROJECT_MFG_API UMFG_HUDHealthBar : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		float HealthPercentage;

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		FLinearColor HealthColor;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
		FLinearColor FullHealthColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
		FLinearColor EmptyHealthColor;

	UPROPERTY(meta = (BindWidgetAnim))
		UWidgetAnimation* HealthBarFlashing;

public:

	UFUNCTION(BlueprintCallable)
	void InitializeWidget();

	UFUNCTION()
	void UpdateHealth(float CurrentHealth, float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_UpdateHealth(float CurrentHealth, float MaxHealth);

};
