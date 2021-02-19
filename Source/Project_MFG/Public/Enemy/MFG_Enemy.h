// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MFG_Character.h"
#include "MFG_Enemy.generated.h"

class AMFG_PathActor;
class AMFG_Item;
class AMFG_AIController;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Navigation Path")
		bool bLoopPath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Navigation Path")
		int DirectionIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Navigation Path")
		float WaitingTimeOnPathPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Ultimate XP")
		float XPValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System")
		float LootProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Navigation Path")
		AMFG_PathActor* MyPath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System")
		TSubclassOf<AMFG_Item> LootItemClass;

	UPROPERTY(BlueprintReadOnly, Category = "AI Controller")
		AMFG_AIController* MyAIController;

	FTimerHandle TimerHandle_DestroyEnemy;

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

};
