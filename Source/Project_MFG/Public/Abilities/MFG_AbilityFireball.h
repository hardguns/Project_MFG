// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/MFG_Ability.h"
#include "MFG_AbilityFireball.generated.h"

class UAnimMontage;
class AMFG_FireballProjectile;
/**
 * 
 */
UCLASS()
class PROJECT_MFG_API AMFG_AbilityFireball : public AMFG_Ability
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
		int FireballsToAppear;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
		float TimeToShot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
		float ProjectileInitialSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
		TSubclassOf<AMFG_FireballProjectile> FireballProjectileClass;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* FireballMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
		UParticleSystem* FireballShockWaveEffect;

	UPROPERTY(BlueprintReadOnly, Category = "Ability")
		TArray<AMFG_FireballProjectile*> CastedFireballs;
		

	FTimerHandle TimerHandle_ShotFireball;

public:

	AMFG_AbilityFireball();

protected:

	virtual void BeginPlay() override;

	virtual void CastAbility(AMFG_Character* AbilityCaster) override;

	virtual void SetAbilityBehavior(AMFG_Character* AbilityCaster) override;

	void ShotFireball();
};
