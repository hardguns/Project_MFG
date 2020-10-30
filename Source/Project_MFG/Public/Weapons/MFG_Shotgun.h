// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/MFG_Weapon.h"
#include "MFG_Shotgun.generated.h"

class UParticleSystem;

/**
 * 
 */
UCLASS()
class PROJECT_MFG_API AMFG_Shotgun : public AMFG_Weapon
{
	GENERATED_BODY()

public:
	
	AMFG_Shotgun();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LineTrace|Debug")
		bool bDrawLineTrace;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LineTrace")
		float TraceLenght;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LineTrace")
		int PelletsNumber;

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
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shotgun")
		float ShotForce;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LineTrace|Debug")
		bool CanShoot;

protected:

	virtual void StartAction() override;

	virtual void StopAction() override;

	void ActionShot();

	float CalculateDamage(int numberOfHits);

};
