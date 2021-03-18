// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MFG_EnemyHealthBar.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MFG_API UMFG_EnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		float HealthPercentage;

public:

	UFUNCTION()
		void UpdateHealth(float CurrentHealth, float MaxHealth);
	
};
