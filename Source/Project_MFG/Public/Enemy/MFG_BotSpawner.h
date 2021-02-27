// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MFG_BotSpawner.generated.h"

class UBillboardComponent;
class AMFG_Bot;
class AMFG_Item;

UCLASS()
class PROJECT_MFG_API AMFG_BotSpawner : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UBillboardComponent* SpawnerBillboardComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
		bool bIsActive;

	UPROPERTY(BlueprintReadOnly, Category = "Spawner")
		bool bHasSpawnedItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (UIMin = 1.0, ClampMin = 1.0))
		int MaxBotsCounter;

	UPROPERTY(BlueprintReadOnly, Category = "Spawner")
		int CurrentBotsCounter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (UIMin = 0.1, ClampMin = 0.1))
		float TimeToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System")
		float LootProbability;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (MakeEditWidget = true))
		TArray<FVector> SpawnPoints;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawner")
		TSubclassOf<AMFG_Bot> BotClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot System")
		TSubclassOf<AMFG_Item> LootItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System")
		FName KeyTag;

	FTimerHandle TimerHandle_SpawnBot;
	
public:	
	// Sets default values for this actor's properties
	AMFG_BotSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnBot();

	FVector GetSpawnPoint();

	bool TrySpawnLoot();

public: 

	void NotifyBotDead();

	void SetActiveState(bool NewState) { bIsActive = NewState; };

	bool GetBotSpawnerState() { return bIsActive; };

	int GetCurrentBotsOnScene() { return CurrentBotsCounter; };

	FName GetKeyTagToSpawn() { return KeyTag; };

};
