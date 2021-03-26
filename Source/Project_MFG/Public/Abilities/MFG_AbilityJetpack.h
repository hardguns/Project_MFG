// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/MFG_Ability.h"
#include "MFG_AbilityJetpack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MFG_API AMFG_AbilityJetpack : public AMFG_Ability
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float DashForce;

protected:

	virtual void BeginPlay() override;

	virtual void CastAbility() override;

	virtual void ReloadAbility() override;
	
};
