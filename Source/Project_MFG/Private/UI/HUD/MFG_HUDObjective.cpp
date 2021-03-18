// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/MFG_HUDObjective.h"

void UMFG_HUDObjective::ObjectiveCompleted()
{
	bIsCompleted = true;
	CurrentColor = CompletedColor;
}
