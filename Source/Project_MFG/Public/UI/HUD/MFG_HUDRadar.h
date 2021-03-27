// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MFG_HUDRadar.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MFG_API UMFG_HUDRadar : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
		FVector2D ScreenSize;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
		float ObjectDistanceScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
		float RadarSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
		FVector2D RadarStartLocation;

	UPROPERTY(BlueprintReadWrite, Category = "Setup")
		TArray<AActor*> RadarActors;

public:

	UFUNCTION(BlueprintCallable)
	void AddToRadar(AActor* RadarActor);

	UFUNCTION(BlueprintCallable)
	void RemoveFromRadar(AActor* RadarActor);
	
};
