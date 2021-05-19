// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MFG_ObjectiveCreator.generated.h"

class UBoxComponent;
class UMFG_HUDObjective;

UCLASS()
class PROJECT_MFG_API AMFG_ObjectiveCreator : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UBoxComponent* ObjectiveZoneComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Objective Setup")
		FName ObjectiveName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Objective Setup")
		FName ObjectiveIdentifier;

	UPROPERTY(BlueprintReadOnly, Category = "Objectives")
		TArray<UMFG_HUDObjective*> NewObjectives;
	
public:	
	// Sets default values for this actor's properties
	AMFG_ObjectiveCreator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void CreateNewObjective(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
