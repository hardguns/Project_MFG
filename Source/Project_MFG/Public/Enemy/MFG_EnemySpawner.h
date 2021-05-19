// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MFG_EnemySpawner.generated.h"

class UBillboardComponent;
class AMFG_Enemy;
class AMFG_PathActor;
class AMFG_GameMode;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemiesSpawnedDefeatedSignature);

UCLASS()
class PROJECT_MFG_API AMFG_EnemySpawner : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UBillboardComponent* SpawnerBillboardComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
		bool bIsActive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
		bool bHasToStopSpawning;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner|Hordes")
		bool bHasToSpawnEnemyHordes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (UIMin = 1.0f, ClampMin = 1.0f))
		int MaxEnemyCounter;

	UPROPERTY(BlueprintReadOnly, Category = "Spawner|Hordes")
		int CurrentHordeEnemyCounter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner|Hordes")
		int MaxEnemyHordes;

	UPROPERTY(BlueprintReadOnly, Category = "Spawner|Hordes")
		int CurrentEnemyHordes;

	UPROPERTY(BlueprintReadOnly, Category = "Spawner")
		int CurrentEnemyCounter;

	UPROPERTY(BlueprintReadOnly, Category = "Spawner")
		TArray<int> DirectionIndexes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (UIMin = 0.1f, ClampMin = 0.1f))
		float TimeToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner|Destructible Object")
		AActor* DestructibleObject;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
		TArray<TSubclassOf<AMFG_Enemy>> EnemyClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (MakeEditWidget = true))
		TArray<FVector> SpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
		TArray<AMFG_PathActor*> EnemyActorPaths;

	AMFG_GameMode* GameModeReference;

	FTimerHandle TimerHandle_SpawnEnemy;

public:

	UPROPERTY(BlueprintAssignable)
		FOnEnemiesSpawnedDefeatedSignature OnEnemiesSpawnedDefeatedDelegate;
	
public:	
	// Sets default values for this actor's properties
	AMFG_EnemySpawner();

	bool GetIsActive() { return bIsActive; };

	void SetIsActive(bool NewValue) { bIsActive = NewValue; };

	void NotifyEnemyDead();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector GetSpawnPoint();

	void SpawnEnemy();

	void NotifyEnemiesDefeated();

};
