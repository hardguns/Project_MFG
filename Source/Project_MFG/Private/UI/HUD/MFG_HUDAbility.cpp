// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/MFG_HUDAbility.h"
#include "MFG_Character.h"
#include "Kismet/GameplayStatics.h"

void UMFG_HUDAbility::InitializeWidget()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (IsValid(PlayerPawn))
	{
		AMFG_Character* PlayerCharacter = Cast<AMFG_Character>(PlayerPawn);
		if (IsValid(PlayerCharacter))
		{
			PlayerCharacter->OnAbilityChangeDelegate.AddDynamic(this, &UMFG_HUDAbility::UpdateAbilityState);
			AbilityAvailable = PlayerCharacter->GetAbilityAmountAvailable();
			AbilityTotalCoolDown = PlayerCharacter->GetAbilityReloadTime();
			MaxAbilityAmount = PlayerCharacter->GetAbilityMaximumAmount();

			AbilityIcon = PlayerCharacter->GetAbilityIcon(AbilityIndex);
		}
	}

	ReloadingBarTime = 0.1f;
	AbilityPercentage = 1.0f;
}

void UMFG_HUDAbility::UpdateAbilityState(int AbilityAmountAvailable)
{
	AbilityAvailable = AbilityAmountAvailable;
	if (AbilityAmountAvailable < MaxAbilityAmount)
	{
		AbilityCurrentCoolDown = 0;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ReloadAbility, this, &UMFG_HUDAbility::ReloadBar, ReloadingBarTime, true);
	}
}

void UMFG_HUDAbility::ReloadBar()
{
	if (AbilityCurrentCoolDown <= AbilityTotalCoolDown)
	{
		AbilityCurrentCoolDown += ReloadingBarTime;
		AbilityPercentage = AbilityCurrentCoolDown / AbilityTotalCoolDown;
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ReloadAbility);
	}
}


