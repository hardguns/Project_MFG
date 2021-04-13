// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/MFG_InteractiveObject.h"
#include "MFG_ElectricityGen.generated.h"

class UStaticMeshComponent;
class UParticleSystem;
class AMFG_EPlatform;
class AMFG_Activable;

/**
 * 
 */
UCLASS()
class PROJECT_MFG_API AMFG_ElectricityGen : public AMFG_InteractiveObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* ElectricityMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		UParticleSystem* ActivateEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Connected Objects")
		TArray<AMFG_EPlatform*> PlaftformsToActivate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Connected Objects")
		TArray<AMFG_InteractiveObject*> LevelElectricityGenerators;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Conected Spawners")
		TArray<AMFG_Activable*> BotSpawnersActivables;

public:

	AMFG_ElectricityGen();

public:

	void ActivateElectricity();

	void ActivateSpawners();

protected:

	virtual void Interact(AMFG_Character* OtherActor) override;

	virtual void StopInteract(AMFG_Character* OtherActor) override;

	virtual void PlayInteractionSound(USoundCue* InteractionSound) override;

	virtual void HitObject() override;
	
};
