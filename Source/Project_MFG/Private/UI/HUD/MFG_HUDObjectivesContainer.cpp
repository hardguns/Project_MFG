// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/MFG_HUDObjectivesContainer.h"
#include "Core/MFG_GameMode.h"
#include "UI/HUD/MFG_HUDObjective.h"
#include "Components/VerticalBox.h"

void UMFG_HUDObjectivesContainer::InitializeWidget()
{
	GameModeReference = Cast<AMFG_GameMode>(GetWorld()->GetAuthGameMode());

	if (IsValid(GameModeReference))
	{
		GameModeReference->OnKeyAddedDelegate.AddDynamic(this, &UMFG_HUDObjectivesContainer::UpdateObjectives);
		GameModeReference->OnEnergyEnabledDelegate.AddDynamic(this, &UMFG_HUDObjectivesContainer::UpdateObjectives);
	}
}

void UMFG_HUDObjectivesContainer::InitializeObjectives(TArray<UMFG_HUDObjective*> NewObjectives)
{
	Objectives.Empty();

	for (UMFG_HUDObjective* NewObjective : NewObjectives)
	{
		Objectives.AddUnique(NewObjective);
	}
}

void UMFG_HUDObjectivesContainer::CreateNewObjectives(TArray<UMFG_HUDObjective*> NewObjectives)
{
	InitializeObjectives(NewObjectives);

	BP_CreateNewObjectives();
}

void UMFG_HUDObjectivesContainer::UpdateObjectives(FName IdentifierTag)
{
	int ObjectiveIndex = -1;

	for (int i = 0; i < Objectives.Num(); i++)
	{
		if (Objectives[i]->GetObjectiveIdentifier() == IdentifierTag)
		{
			ObjectiveIndex = i;
			break;
		}
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
