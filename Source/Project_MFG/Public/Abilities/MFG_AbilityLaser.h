// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/MFG_Ability.h"
#include "MFG_AbilityLaser.generated.h"

class AMFG_LaserProjectile;
class UAnimMontage;

UCLASS()
class PROJECT_MFG_API AMFG_AbilityLaser : public AMFG_Ability
{
	GENERATED_BODY()

public:

	AMFG_AbilityLaser();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
		float LaserTraceLenght;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
		float LaserReloadCooldown;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Ability")
		FName LaserSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		TSubclassOf<AMFG_LaserProjectile> LaserProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* LaserShotMontage;

public: 

	virtual void BeginPlay() override;

	virtual void CastAbility() override;

	virtual void ReloadAbility() override;

	virtual void SetAbilityBehavior() override;
	
};
