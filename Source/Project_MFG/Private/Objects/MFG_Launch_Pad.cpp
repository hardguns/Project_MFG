// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/MFG_Launch_Pad.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "MFG_Character.h"

AMFG_Launch_Pad::AMFG_Launch_Pad() 
{
	LaunchPadMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaunchPadMeshComponent"));
	LaunchPadMeshComponent->SetupAttachment(RootComponent);
	LaunchPadMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	LaunchPadParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaunchPadParticleComponent"));
	LaunchPadParticleComponent->SetupAttachment(RootComponent);
	LaunchPadParticleComponent->SetVisibility(false);

	LaunchForce = 1000.0f;
	InitialLaunchDirection = FVector::UpVector;
}

void AMFG_Launch_Pad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSwitchState)
	{
		LaunchPadParticleComponent->SetVisibility(true);
	}
	else
	{
		LaunchPadParticleComponent->SetVisibility(false);
	}
}

void AMFG_Launch_Pad::Interact(AMFG_Character* CharacterToLaunch)
{
	Super::Interact(CharacterToLaunch);

	if (bSwitchState) {
		FVector LaunchDirection = (GetRootComponent()->GetForwardVector() + InitialLaunchDirection);
		//CharacterToLaunch->UnCrouch();
		CharacterToLaunch->LaunchCharacter(LaunchDirection * LaunchForce, true, true);
	}
}



