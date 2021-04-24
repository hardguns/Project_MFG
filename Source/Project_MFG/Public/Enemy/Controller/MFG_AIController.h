// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MFG_AIController.generated.h"

class AMFG_Enemy;
class UBlackboardComponent;
class UBehaviorTree;
class UAIPerceptionComponent;

/**
 * 
 */
UCLASS()
class PROJECT_MFG_API AMFG_AIController : public AAIController
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UAIPerceptionComponent* AIPerceptionComponent;

protected:

	UPROPERTY(BlueprintReadWrite, Category = "Enemy Controller")
		bool bReceivingDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Controller")
		FName LoopPathParameterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Controller")
		FName DirectionIndexParameter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Controller")
		FName WaitingTimeParameterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Controller")
		FName InvestigatingParameterName;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Controller")
		FName CanSeePlayerParameterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Controller")
		FName TargetLocationParameterName;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy Controller")
		AMFG_Enemy* MyEnemy;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy Controller")
		UBlackboardComponent* MyBlackboard;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy Controller")
		UBehaviorTree* EnemyBehaviorTree;

public:
	// Sets default values for this character's properties
	AMFG_AIController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	void SetBlackboardValues(APawn* EnemyPawn);

	UFUNCTION()
	void UpdateSenses(const TArray<AActor*>& UpdatedActors);

public:

	void SetReceiveDamage(bool bNewState) { bReceivingDamage = bNewState; };

	void DeactivateAIPerception();
	
};
