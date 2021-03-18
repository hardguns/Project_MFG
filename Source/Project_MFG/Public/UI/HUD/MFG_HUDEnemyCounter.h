// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MFG_HUDEnemyCounter.generated.h"

class UMFG_GameInstance;

UCLASS()
class PROJECT_MFG_API UMFG_HUDEnemyCounter : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		int EnemiesDefeated;

	UPROPERTY(BlueprintReadOnly, Category = "Setup")
		UMFG_GameInstance* GameInstanceReference;
	
public:

	UFUNCTION(BlueprintCallable)
	void InitializeWidget();

	UFUNCTION()
	void UpdateCounter(int EnemyDefeatedCounter);
};
