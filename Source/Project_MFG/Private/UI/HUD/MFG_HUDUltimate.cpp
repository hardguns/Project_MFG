// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/MFG_HUDUltimate.h"
#include "Kismet/GameplayStatics.h"
#include "MFG_Character.h"

void UMFG_HUDUltimate::InitializeWidget()
{
	UltimateColor = UltimateDisabledColor;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (IsValid(PlayerPawn))
	{
		AMFG_Character* PlayerCharacter = Cast<AMFG_Character>(PlayerPawn);
		if (IsValid(PlayerCharacter))
		{
			PlayerCharacter->OnUltimateUpdateDelegate.AddDynamic(this, &UMFG_HUDUltimate::UpdateUltimateValue);
			PlayerCharacter->OnUltimateStatusDelegate.AddDynamic(this, &UMFG_HUDUltimate::UpdateUltimateStatus);
		}
	}
}

void UMFG_HUDUltimate::UpdateUltimateValue(float CurrentUltimateXP, float MaxUltimateXP)
{
	UltimatePercentage = CurrentUltimateXP / MaxUltimateXP;
}

void UMFG_HUDUltimate::UpdateUltimateStatus(bool bIsAvailable)
{
	UltimateColor = bIsAvailable ? UltimateEnabledColor : UltimateDisabledColor;
}
