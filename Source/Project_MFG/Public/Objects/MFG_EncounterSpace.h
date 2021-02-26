// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MFG_EncounterSpace.generated.h"

class UBoxComponent;

UCLASS()
class PROJECT_MFG_API AMFG_EncounterSpace : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
		UBoxComponent* EncounterCollider;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Location")
		FVector PossiblePlayerLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Location")
		FName TargetLocationParameterName;
	
public:	
	// Sets default values for this actor's properties
	AMFG_EncounterSpace();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	void CalloutTarget(FVector TargetLocation);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Encounter Space")
	void BP_CalloutTarget();

};
