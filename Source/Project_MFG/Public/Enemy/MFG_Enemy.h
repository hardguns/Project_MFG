// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MFG_Character.h"
#include "MFG_Enemy.generated.h"

class AMFG_PathActor;
class AMFG_Item;
class AMFG_AIController;
class UWidgetComponent; 
class UMFG_EnemyHealthBar;
class AMFG_EnemySpawner;

/**
 *
 */
UCLASS()
class PROJECT_MFG_API AMFG_Enemy : public AMFG_Character
{
	GENERATED_BODY()

public:

	AMFG_Enemy();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UWidgetComponent* WidgetHealthBarComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Navigation Path")
		bool bLoopPath; 
		
	UPROPERTY(BlueprintReadOnly, Category = "UI")
		bool bIsShowingHealthBar;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
		bool bIsAlert;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Navigation Path")
		int DirectionIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Navigation Path")
		float WaitingTimeOnPathPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Ultimate XP")
		float XPValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System")
		float LootProbability;

	UPROPERTY(BlueprintReadOnly, Category = "Spawner")
		AMFG_EnemySpawner* MyEnemySpawner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Navigation Path")
		AMFG_PathActor* MyPath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System")
		TSubclassOf<AMFG_Item> LootItemClass;

	UPROPERTY(BlueprintReadOnly, Category = "AI Controller")
		AMFG_AIController* MyAIController;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
		UMFG_EnemyHealthBar* EnemyHealthBar;

	FTimerHandle TimerHandle_DestroyEnemy;

	FTimerHandle TimerHandle_HideHealthBar;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	UFUNCTION()
	void GiveXP(AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_GiveXP(AActor* DamageCauser);

	bool TrySpawnLoot();

	UFUNCTION()
	void HealthChanged(UMFG_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	void DestroyEnemy();

public:

	bool GetLoopPath() { return bLoopPath; };
	int GetDirectionIndex() { return DirectionIndex; };
	float GetWaitingTime() { return WaitingTimeOnPathPoint; };

	void SetDirectionIndex(int NewDirectionIndex) { DirectionIndex = NewDirectionIndex; };

	void ShowHealthBar();
	void HideHealthBar();

	bool IsAlert(){ return bIsAlert; };
	void SetAlert(bool bValue);

	void SetEnemySpawner(AMFG_EnemySpawner* NewSpawner) { MyEnemySpawner = NewSpawner; };
	void SetPathActor(AMFG_PathActor* NewPathActor) { MyPath = NewPathActor; };

};
