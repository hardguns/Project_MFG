// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/MFG_AbilityJetpack.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MFG_Character.h"

void AMFG_AbilityJetpack::BeginPlay()
{
	Super::BeginPlay();

	//Initialize ability use amount
	AbilityDetails.CurrentAbilityUseAmount = AbilityDetails.MaximumAbilityUseAmount;
}

void AMFG_AbilityJetpack::CastAbility(AMFG_Character* AbilityCaster)
{
	Super::CastAbility(AbilityCaster);

	if (IsValid(PlayerCharacterReference))
	{
		//Checks if player can use ability
		if (AbilityDetails.CurrentAbilityUseAmount > 0)
		{
			//Cast to ACharacter obj to get GetMovementComponent from player
			ACharacter* PlayerCharacter = Cast<ACharacter>(PlayerCharacterReference);
			if (IsValid(PlayerCharacter))
			{
				UPawnMovementComponent* Movement = PlayerCharacter->GetMovementComponent();
				//Checks if player is falling 
				if (PlayerCharacterReference->GetIsUsingBag() || Movement->IsFalling())
				{
					return;
				}
			}
			
			//Jetpack launches player to certain location using a Dash force
			FVector currentPosition = PlayerCharacterReference->GetCurrentPosition();
			PlayerCharacterReference->LaunchCharacter(currentPosition * DashForce, true, true);
			PlayerCharacterReference->SetIsUsingBag(true);

			AbilityDetails.CurrentAbilityUseAmount--;

			//Broadcast abilityChangeDelegate in order to notify to widget and AnimNotify that an ability was used 
			PlayerCharacterReference->OnAbilityChangeDelegate.Broadcast(AbilityDetails.CurrentAbilityUseAmount, AbilityDetails.AbilityIndex, true);

			//Start reloading ability
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_ReloadAbility, this, &AMFG_AbilityJetpack::ReloadAbility, AbilityDetails.AbilityCooldown, true);
		}
	}

}

void AMFG_AbilityJetpack::ReloadAbility()
{
	//Checks if ability was reloaded at maximum
	if (AbilityDetails.CurrentAbilityUseAmount < AbilityDetails.MaximumAbilityUseAmount)
	{
		AbilityDetails.CurrentAbilityUseAmount++;
		if (IsValid(PlayerCharacterReference))
		{
			//Broadcast abilityChangeDelegate in order to notify to widget that an ability was used
			PlayerCharacterReference->OnAbilityChangeDelegate.Broadcast(AbilityDetails.CurrentAbilityUseAmount, AbilityDetails.AbilityIndex, true);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ReloadAbility);
	}
}
