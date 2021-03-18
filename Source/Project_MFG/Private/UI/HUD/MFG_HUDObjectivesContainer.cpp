// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/MFG_HUDObjectivesContainer.h"
#include "Core/MFG_GameMode.h"
#include "UI/HUD/MFG_HUDObjective.h"

void UMFG_HUDObjectivesContainer::InitializeWidget()
{
	GameModeReference = Cast<AMFG_GameMode>(GetWorld()->GetAuthGameMode());

	if (IsValid(GameModeReference))
	{
		GameModeReference->OnKeyAddedDelegate.AddDynamic(this, &UMFG_HUDObjectivesContainer::UpdateObjectives);
	}
}

void UMFG_HUDObjectivesContainer::InitializeObjectives(TArray<UMFG_HUDObjective*> NewObjectives)
{
	for (UMFG_HUDObjective* NewObjective : NewObjectives)
	{
		Objectives.AddUnique(NewObjective);
	}
}

void UMFG_HUDObjectivesContainer::UpdateObjectives(FName KeyTag)
{
	int ObjectiveIndex = -1;

	if (KeyTag == "KeyA")
	{
		ObjectiveIndex = 0;
	}

	if (KeyTag == "KeyB")
	{
		ObjectiveIndex = 1;
	}

	if (Objectives.IsValidIndex(ObjectiveIndex))
	{
		UMFG_HUDObjective* CurrentObjective = Objectives[ObjectiveIndex];

		if (IsValid(CurrentObjective))
		{
			CurrentObjective->ObjectiveCompleted();
		}
		
	}
}
