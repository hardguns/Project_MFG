// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MFG_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MFG_API UMFG_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UMFG_GameInstance();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Save System")
		FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = "Enemy Data")
		int EnemiesDefeatedCounter;
		
public:

	UFUNCTION(BlueprintCallable)
	int GetEnemiesDefeatedCounter() { return EnemiesDefeatedCounter; };

	UFUNCTION(BlueprintCallable)
	void SetEnemiesDefeatedCounter(int NewValue) { EnemiesDefeatedCounter = NewValue; };

	UFUNCTION(BlueprintCallable)
	void AddEnemyDefeatedToCounter();

	UFUNCTION(BlueprintCallable)
	void SaveData();

	UFUNCTION(BlueprintCallable)
	void LoadData();

	UFUNCTION(BlueprintCallable)
	void ResetData();

protected:

	UFUNCTION(BlueprintImplementableEvent)
	void BP_AddEnemyDefeatedToCounter();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_SaveData();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_LoadData();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_ResetData();
	
};
