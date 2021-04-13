// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/MFG_HUDAbility.h"
#include "MFG_Character.h"
#include "Abilities/MFG_Ability.h"
#include "Kismet/GameplayStatics.h"

void UMFG_HUDAbility::Start()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_LoadWidget, this, &UMFG_HUDAbility::InitializeWidget, StartDelayDuration, false);
}

void UMFG_HUDAbility::InitializeWidget()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (IsValid(PlayerPawn))
	{
		AMFG_Character* PlayerCharacter = Cast<AMFG_Character>(PlayerPawn);
		if (IsValid(PlayerCharacter))
		{
			PlayerCharacter->OnAbilityChangeDelegate.AddDynamic(this, &UMFG_HUDAbility::UpdateAbilityState);
			CharacterAbilities = PlayerCharacter->GetCharacterAbilities();
			if (CharacterAbilities.IsValidIndex(AbilityIndex))
			{
				if (IsValid(CharacterAbilities[AbilityIndex]))
				{
					FMFG_AbilityStruct AbilityDetails = CharacterAbilities[AbilityIndex]->GetAbilityDetails();
					MaxAbilityAmount = AbilityDetails.MaximumAbilityUseAmount;
					CurrentAbilityAmount = MaxAbilityAmount;
					AbilityTotalCoolDown = AbilityDetails.AbilityCooldown;
					AbilityIcon = AbilityDetails.AbilityIcon;
				}
			}
		}
	}

	ReloadingBarSpeed = 0.1f;
	AbilityPercentage = 0;
}

void UMFG_HUDAbility::UpdateAbilityState(int AbilityAmountAvailable, int ReceivedAbilityIndex, bool bShowReloadingBar)
{
	if (AbilityIndex == ReceivedAbilityIndex)
	{
		CurrentAbilityAmount = AbilityAmountAvailable;
		if (CurrentAbilityAmount < MaxAbilityAmount)
		{
			AbilityCurrentCoolDown = AbilityTotalCoolDown;

			if (bShowReloadingBar)
			{
				GetWorld()->GetTimerManager().SetTimer(TimerHandle_ReloadAbility, this, &UMFG_HUDAbility::ReloadBar, ReloadingBarSpeed, true);
			}
		}
	}
}

void UMFG_HUDAbility::ReloadBar()
{
	if (AbilityCurrentCoolDown >= 0)
	{
		AbilityCurrentCoolDown -= ReloadingBarSpeed;
		AbilityPercentage = AbilityCurrentCoolDown / AbilityTotalCoolDown;
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ReloadAbility);
	}
}


