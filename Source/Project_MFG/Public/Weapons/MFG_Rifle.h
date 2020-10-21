// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/MFG_Weapon.h"
#include "MFG_Rifle.generated.h"

class UParticleSystem;

/**
 * 
 */
UCLASS()
class PROJECT_MFG_API AMFG_Rifle : public AMFG_Weapon
{
	GENERATED_BODY()

public:

	AMFG_Rifle();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon options")
		bool bIsAutomatic;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LineTrace|Debug")
		bool bDrawLineTrace;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LineTrace")
		float TraceLenght;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon options")
		float TimeBetweenShots;

	FTimerHandle TimerHandle_HandleAutomaticShot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		FName TraceParamName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		UParticleSystem* ImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		UParticleSystem* TraceEffect;	

protected:

	virtual void StartAction() override;

	virtual void StopAction() override;

	void ActionShot();
	
};
