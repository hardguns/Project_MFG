// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/MFG_Weapon.h"
#include "GameFramework/Character.h"

// Sets default values
AMFG_Weapon::AMFG_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Damage = 20.0f;
}

// Called when the game starts or when spawned
void AMFG_Weapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMFG_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMFG_Weapon::StartAction()
{
	BP_StartAction();
}

void AMFG_Weapon::StopAction()
{
	BP_StopAction();
}

void AMFG_Weapon::SetCharacterOwner(ACharacter* NewOwner)
{
	if (IsValid(NewOwner))
	{
		SetOwner(NewOwner);
		CurrentOwnerCharacter = NewOwner;
	}
}

void AMFG_Weapon::SetCurrentDamage(float NewDamage)
{
	Damage = NewDamage;
}

