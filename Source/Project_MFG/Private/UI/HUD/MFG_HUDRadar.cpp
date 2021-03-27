// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/MFG_HUDRadar.h"

void UMFG_HUDRadar::AddToRadar(AActor* RadarActor)
{
	RadarActors.Add(RadarActor);
}

void UMFG_HUDRadar::RemoveFromRadar(AActor* RadarActor)
{
	RadarActors.Remove(RadarActor);
}
