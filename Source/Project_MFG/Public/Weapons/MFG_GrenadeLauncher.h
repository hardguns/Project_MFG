// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/MFG_Weapon.h"
#include "MFG_GrenadeLauncher.generated.h"

class AMFG_Projectile;

/**
 * 
 */
UCLASS()
class PROJECT_MFG_API AMFG_GrenadeLauncher : public AMFG_Weapon
{
	GENERATED_BODY()

public: 

	AMFG_GrenadeLauncher();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		FName MuzzleSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grenade Launcher")
		float TraceLenght;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grenade Launcher")
		TSubclassOf<AMFG_Projectile> ProjectileClass;

protected:

	virtual void StartAction() override;

	virtual void StopAction() override;
	
};
