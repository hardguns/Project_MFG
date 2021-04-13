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

void AMFG_Ability::CastAbility()
{
	BP_CastAbility();
}

void AMFG_Ability::SetAbilityBehavior()
{
	BP_SetAbilityBehavior();
}

void AMFG_Ability::StopCastingAbility()
{
	BP_StopCastingAbility();
}

void AMFG_Ability::ReloadAbility()
{
	BP_ReloadAbility();
}

