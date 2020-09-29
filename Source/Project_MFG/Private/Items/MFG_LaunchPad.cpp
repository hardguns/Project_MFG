// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/MFG_LaunchPad.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "MFG_Character.h"

AMFG_LaunchPad::AMFG_LaunchPad()
{
	bIsActive = false;

	LaunchPadMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LaunchPadMeshComponent"));
	LaunchPadMeshComponent->SetupAttachment(RootComponent);
	LaunchPadMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	LaunchPadParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaunchPadParticleComponent"));
	LaunchPadParticleComponent->SetupAttachment(RootComponent);
	LaunchPadParticleComponent->SetVisibility(false);

	LaunchForce = 1000.0f;
	LaunchPadTag = "LaunchPad1";
}

void AMFG_LaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsActive)
	{
		LaunchPadParticleComponent->SetVisibility(true);
	}
	else 
	{
		LaunchPadParticleComponent->SetVisibility(false);
	}
}

void AMFG_LaunchPad::Pickup(AMFG_Character* CharacterToLaunch)
{
	Super::Pickup(CharacterToLaunch);

	if (bIsActive) {
		FVector LaunchDirection = (GetRootComponent()->GetForwardVector() + FVector(0, 0, 1.0f));
		//CharacterToLaunch->UnCrouch();
		CharacterToLaunch->LaunchCharacter(LaunchDirection * LaunchForce, true, true);
	}
}

