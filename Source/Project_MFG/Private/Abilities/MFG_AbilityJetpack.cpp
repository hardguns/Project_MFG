// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/MFG_AbilityJetpack.h"
#include "MFG_Character.h"

void AMFG_AbilityJetpack::BeginPlay()
{
	Super::BeginPlay();

	AbilityDetails.CurrentAbilityUseAmount = AbilityDetails.MaximumAbilityUseAmount;
}

void AMFG_AbilityJetpack::CastAbility()
{
	Super::CastAbility();

	if (IsValid(PlayerCharacterReference))
	{
		if (AbilityDetails.CurrentAbilityUseAmount > 0)
		{
			FVector currentPosition = PlayerCharacterReference->GetCurrentPosition();
			PlayerCharacterReference->LaunchCharacter(currentPosition * DashForce, true, true);
			PlayerCharacterReference->SetIsUsingBag(true);

			AbilityDetails.CurrentAbilityUseAmount--;

			PlayerCharacterReference->OnAbilityChangeDelegate.Broadcast(AbilityDetails.CurrentAbilityUseAmount, AbilityDetails.AbilityIndex);

			GetWorld()->GetTimerManager().SetTimer(TimerHandle_ReloadAbility, this, &AMFG_AbilityJetpack::ReloadAbility, AbilityDetails.AbilityCooldown, true);
		}
	}

}

void AMFG_AbilityJetpack::ReloadAbility()
{
	if (AbilityDetails.CurrentAbilityUseAmount < AbilityDetails.MaximumAbilityUseAmount)
	{
		AbilityDetails.CurrentAbilityUseAmount++;
		if (IsValid(PlayerCharacterReference))
		{
			PlayerCharacterReference->OnAbilityChangeDelegate.Broadcast(AbilityDetails.CurrentAbilityUseAmount, AbilityDetails.AbilityIndex);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ReloadAbility);
	}
}
