// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MFG_EnemySpawner.generated.h"

class UBillboardComponent;
class AMFG_Enemy;
class AMFG_PathActor;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (UIMin = 1.0f, ClampMin = 1.0f))
		int MaxEnemyCounter;

	UPROPERTY(BlueprintReadOnly, Category = "Spawner")
		int CurrentEnemyCounter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (UIMin = 0.1f, ClampMin = 0.1f))
		float TimeToSpawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawner")
		TArray<TSubclassOf<AMFG_Enemy>> EnemyClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner", meta = (MakeEditWidget = true))
		TArray<FVector> SpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
		TArray<AMFG_PathActor*> EnemyActorPaths;

	FTimerHandle TimerHandle_SpawnEnemy;
	
public:	
	// Sets default values for this actor's properties
	AMFG_EnemySpawner();

	void NotifyEnemyDead();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector GetSpawnPoint();

	void SpawnEnemy();

};
