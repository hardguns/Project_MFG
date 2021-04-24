// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MFG_ZoneObjectLocker.generated.h"

class UBoxComponent;
class AMFG_Door;
class AMFG_EnemySpawner;

UCLASS()
class PROJECT_MFG_API AMFG_ZoneObjectLocker : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UBoxComponent* ActivableZoneComponent;

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Zone Trigger")
		bool bWasTriggered;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger Objects")
		TArray<AMFG_Door*> DoorsToTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trigger Objects")
		TArray<AMFG_EnemySpawner*> EnemySpawnersToTrigger;
	
public:	
	// Sets default values for this actor's properties
	AMFG_ZoneObjectLocker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void TriggerObjects();

	UFUNCTION()
	void TriggerContinueAction();

	UFUNCTION()
	void TriggerActivateAction(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
