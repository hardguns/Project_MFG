// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MFG_RadarComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_MFG_API UMFG_RadarComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMFG_RadarComponent();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Behaviour")
		bool bIsStatic;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Behaviour")
		UTexture2D* IconImage;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type Reason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
