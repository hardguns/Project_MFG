// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/MFG_ElectricityGen.h"
#include "MFG_Character.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "MFG_EPlatform.h"
#include "Objects/MFG_Activable.h"
#include "Core/MFG_GameMode.h"

AMFG_ElectricityGen::AMFG_ElectricityGen() 
{
	ElectricityMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ElectricityMeshComponent"));
	ElectricityMeshComponent->SetupAttachment(RootComponent);
}

void AMFG_ElectricityGen::Interact(AMFG_Character* OtherActor)
{
	Super::Interact(OtherActor);

	if (IsValid(OtherActor))
	{
		AMFG_Character* OverlappedCharacter = Cast<AMFG_Character>(OtherActor);
		if (IsValid(OverlappedCharacter))
		{
			OverlappedCharacter->InteractiveObject = this;
		}
	}
}

void AMFG_ElectricityGen::StopInteract(AMFG_Character* OtherActor)
{
	Super::StopInteract(OtherActor);

	if (IsValid(OtherActor))
	{
		AMFG_Character* OverlappedCharacter = Cast<AMFG_Character>(OtherActor);
		if (IsValid(OverlappedCharacter))
		{
			OverlappedCharacter->InteractiveObject = NULL;
		}
	}
}

void AMFG_ElectricityGen::PlayInteractionSound(USoundCue* ObjectSound)
{
	Super::PlayInteractionSound(ObjectSound);
}

void AMFG_ElectricityGen::HitObject()
{
	Super::HitObject();

	ActivateElectricity();
}

void AMFG_ElectricityGen::ActivateElectricity()
{
	if (!bSwitchState)
	{
		bSwitchState = true;

		PlayInteractionSound(InteractionSound);

		if (IsValid(ActivateEffect))
		{
			UParticleSystemComponent* effectComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateEffect, GetActorTransform());
		}

		if (PlaftformsToActivate.Num() > 0)
		{
			bool areGensActive = false;
			for (int i = 0; i < LevelElectricityGenerators.Num(); i++)
			{
				AMFG_InteractiveObject* ElectricityGenerator = Cast<AMFG_InteractiveObject>(LevelElectricityGenerators[i]);
				if (IsValid(ElectricityGenerator))
				{
					if (!ElectricityGenerator->bSwitchState)
					{
						areGensActive = false;
						break;
					}
					else 
					{
						areGensActive = true;
					}
				}
			}

			if (bSwitchState && areGensActive)
			{
				for (int i = 0; i < PlaftformsToActivate.Num(); i++)
				{
					AMFG_EPlatform* Platform = Cast<AMFG_EPlatform>(PlaftformsToActivate[i]);
					if (IsValid(Platform))
					{
						Platform->SetActiveState(true);
					}
				}

				GameModeReference->EnableElectricity(ObjectiveIdentifier);
				ActivateSpawners();
			}
		}

	}
}

void AMFG_ElectricityGen::ActivateSpawners()
{
	if (BotSpawnersActivables.Num() > 0)
	{
		for (AMFG_Activable* BotSpawnerActivable : BotSpawnersActivables)
		{
			BotSpawnerActivable->UseActivable();
		}
	}
}
