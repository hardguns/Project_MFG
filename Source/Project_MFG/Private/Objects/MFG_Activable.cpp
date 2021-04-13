// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/MFG_Activable.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/AudioComponent.h"
#include "MFG_Character.h"
#include "Objects/MFG_Launch_Pad.h"
#include "Enemy/MFG_BotSpawner.h"

AMFG_Activable::AMFG_Activable()
{
	ActivatorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ActivatorMeshComponent"));
	ActivatorMeshComponent->SetupAttachment(RootComponent);

	LightIntensity = 2000.0f;

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightComponent"));
	PointLight->Intensity = LightIntensity;
	//PointLight->SetLightColor(FLinearColor(222,0,0,0), true);
	PointLight->SetupAttachment(RootComponent);

	ActivableTag = "ActivableA";
	InteractionParamName = "Activable";
}

void AMFG_Activable::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(PointLight))
	{
		PointLight->SetLightColor(bSwitchState ? FLinearColor(0.0091f, 0.33f, 0.0f) : FLinearColor(0.49f, 0.0f, 0.0f), true);
	}
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

void AMFG_Activable::PlayInteractionSound(USoundCue* ActivableSound)
{
	Super::PlayInteractionSound(ActivableSound);
}

void AMFG_Activable::ChangeActivableSound(bool bIsActive)
{
	InteractiveAudioComponent->SetBoolParameter(InteractionParamName, bIsActive);
	PlayInteractionSound(InteractionSound);
}

void AMFG_Activable::CheckActivable(AMFG_Character* OtherActor)
{
	if (IsValid(OtherActor) && OtherActor->GetCharacterType() == EMFG_CharacterType::CharacterType_Player)
	{
		if (bNeedsKey)
		{
			if (OtherActor->HasKey(ActivableTag) && !bWasActivated)
			{
				UseActivable();
				OtherActor->RemoveKey(ActivableTag);

				//Was activated variable works only if a activable can be activated only once
				if (bCanBeActivatedOnce)
				{
					bWasActivated = true;
				}
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Needs key: %s or it was activated"), *ActivableTag.ToString());
			}
		}
		else
		{
			UseActivable();
		}
	}

	BP_UseActivator();
}

void AMFG_Activable::UseActivable()
{
	bSwitchState = !bSwitchState;

	if (IsValid(PointLight))
	{
		PointLight->SetLightColor(bSwitchState ? FLinearColor(0.0091f, 0.33f, 0.0f) : FLinearColor(0.49f, 0.0f, 0.0f), true);
	}

	ChangeActivableSound(bSwitchState);

	for (int i = 0; i < ActivableObjects.Num(); i++)
	{
		//Looks for an Interactive object or a bot spawner to activate or deactivated;
		AMFG_InteractiveObject* ActivableObject = Cast<AMFG_InteractiveObject>(ActivableObjects[i]);
		if (IsValid(ActivableObject))
		{
			ActivableObject->bSwitchState = bSwitchState;
		}

		AMFG_BotSpawner* ActivableSpawner = Cast<AMFG_BotSpawner>(ActivableObjects[i]);
		if (IsValid(ActivableSpawner))
		{
			ActivableSpawner->SetActiveState(bSwitchState);
		}
	}
}
