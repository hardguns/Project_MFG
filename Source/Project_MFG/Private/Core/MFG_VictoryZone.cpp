// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MFG_VictoryZone.h"
#include "Components/BoxComponent.h"
#include "MFG_Character.h"
#include "Core/MFG_GameMode.h"

// Sets default values
AMFG_VictoryZone::AMFG_VictoryZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VictoryZoneComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("VictoryZoneComponent"));
	RootComponent = VictoryZoneComponent;
	VictoryZoneComponent->SetBoxExtent(FVector(100.0f));

}

// Called when the game starts or when spawned
void AMFG_VictoryZone::BeginPlay()
{
	Super::BeginPlay();
	
	GameModeReference = Cast<AMFG_GameMode>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void AMFG_VictoryZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMFG_VictoryZone::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (IsValid(OtherActor) && IsValid(GameModeReference))
	{
		AMFG_Character* MFGCharacter = Cast<AMFG_Character>(OtherActor);
		if (IsValid(MFGCharacter) && MFGCharacter->GetCharacterType() == EMFG_CharacterType::CharacterType_Player)
		{
			GameModeReference->Victory(MFGCharacter);
		}
	}
}

