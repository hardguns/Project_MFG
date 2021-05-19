// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/MFG_Ability.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "MFG_Character.h"

// Sets default values
AMFG_Ability::AMFG_Ability()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMFG_Ability::BeginPlay()
{
	Super::BeginPlay();

	//Gets player reference placed in world
	ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (IsValid(Character))
	{
		PlayerCharacterReference = Cast<AMFG_Character>(Character);
	}
}

// Called every frame
void AMFG_Ability::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*------------------------------------------ Ability behavior -------------------------------------------- 
* As parent class these functions let programmer to add general behavior to abilities
*/

//Sets the action when player or enemy is casting an ability
void AMFG_Ability::CastAbility(AMFG_Character* AbilityCaster)
{
	BP_CastAbility(AbilityCaster);
}

//If some extra behavior is necessary in ability, it can be set here 
void AMFG_Ability::SetAbilityBehavior(AMFG_Character* AbilityCaster)
{
	BP_SetAbilityBehavior(AbilityCaster);
}

//Avoid player or enemy to cast an ability (this works can work with long casting abilities)
void AMFG_Ability::StopCastingAbility()
{
	BP_StopCastingAbility();
}

//Use in case that ability use amount must be reloaded 
void AMFG_Ability::ReloadAbility()
{
	BP_ReloadAbility();
}

