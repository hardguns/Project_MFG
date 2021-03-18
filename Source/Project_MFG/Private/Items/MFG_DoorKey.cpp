// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/MFG_DoorKey.h"
#include "Components/StaticMeshComponent.h"
#include "Core/MFG_GameMode.h"
#include "MFG_Character.h"

AMFG_DoorKey::AMFG_DoorKey()
{
	KeyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMeshComponent"));
	KeyMeshComponent->SetupAttachment(RootComponent);
	KeyMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	XPValue = 40.0f;
	KeyTag = "KeyA";
}

void AMFG_DoorKey::Pickup(AMFG_Character* PickupCharacter)
{
	Super::Pickup(PickupCharacter);

	if (IsValid(PickupCharacter) && PickupCharacter->GetCharacterType() == EMFG_CharacterType::CharacterType_Player)
	{
		if (IsValid(GameModeReference))
		{
			GameModeReference->AddKeyToCharacter(PickupCharacter, KeyTag);
		}

		Destroy();
	}
}
