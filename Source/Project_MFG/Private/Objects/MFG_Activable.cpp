// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/MFG_Activable.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "MFG_Character.h"
#include "Objects/MFG_Launch_Pad.h"

AMFG_Activable::AMFG_Activable() 
{
	ActivatorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ActivatorMeshComponent"));
	ActivatorMeshComponent->SetupAttachment(RootComponent);

	LightIntensity = 2000.0f;

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightComponent"));
	PointLight->Intensity = LightIntensity;
	//PointLight->SetLightColor(FLinearColor(222,0,0,0), true);
	PointLight->SetupAttachment(RootComponent);
}

void AMFG_Activable::Interact(AMFG_Character* OtherActor)
{
	Super::Interact(OtherActor);

	if (IsValid(OtherActor))
	{
		AMFG_Character* OverlappedCharacter = Cast<AMFG_Character>(OtherActor);
		if (IsValid(OverlappedCharacter))
		{
			OverlappedCharacter->InteractiveObject = this;
		}
	}
}

void AMFG_Activable::StopInteract(AMFG_Character* OtherActor)
{
	Super::StopInteract(OtherActor);

	if (IsValid(OtherActor))
	{
		AMFG_Character* OverlappedCharacter = Cast<AMFG_Character>(OtherActor);
		if (IsValid(OverlappedCharacter))
		{
			OverlappedCharacter->InteractiveObject = NULL;
		}
	}
}

void AMFG_Activable::UseActivable()
{
	bSwitchState = !bSwitchState;
	for (int i = 0; i < ActivableObjects.Num(); i++)
	{
		AMFG_InteractiveObject* ActivableObject = Cast<AMFG_InteractiveObject>(ActivableObjects[i]);
		if (IsValid(ActivableObject))
		{
			ActivableObject->bSwitchState = bSwitchState;
			//bSwitchState = !activeState;
		}
	}

	BP_UseActivator();
}
