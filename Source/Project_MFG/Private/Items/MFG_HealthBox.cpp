// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/MFG_HealthBox.h"
#include "Components/StaticMeshComponent.h"
#include "MFG_Character.h"

AMFG_HealthBox::AMFG_HealthBox()
{
	HealthBoxMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealthBoxMeshComponent"));
	HealthBoxMeshComponent->SetupAttachment(RootComponent);
	HealthBoxMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HealthValue = 15.0f;
}

void AMFG_HealthBox::Pickup(AMFG_Character* PickupCharacter)
{
	Super::Pickup(PickupCharacter);

	bool bSuccessfulHeal = PickupCharacter->TryAddHealth(HealthValue);

	if (bSuccessfulHeal)
	{
		PlayPickupSound();

		Destroy();
	}
}

