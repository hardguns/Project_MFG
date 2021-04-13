// Fill out your copyright notice in the Description page of Project Settings.


#include "Music/MFG_LevelMusic.h"
#include "Core/MFG_GameMode.h"
#include "Components/BillboardComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
AMFG_LevelMusic::AMFG_LevelMusic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MusicBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("MusicBillboardComponent"));
	RootComponent = MusicBillboardComponent;

	MusicAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MusicAudioComponent"));
	MusicAudioComponent->SetupAttachment(RootComponent);

	AlertParamName = "Alert";

}

// Called when the game starts or when spawned
void AMFG_LevelMusic::BeginPlay()
{
	Super::BeginPlay();

	GameModeReference = Cast<AMFG_GameMode>(GetWorld()->GetAuthGameMode());
	if (IsValid(GameModeReference))
	{
		GameModeReference->OnVictoryDelegate.AddDynamic(this, &AMFG_LevelMusic::StopLevelMusic);
		GameModeReference->OnGameOverDelegate.AddDynamic(this, &AMFG_LevelMusic::StopLevelMusic);
		GameModeReference->OnAlertModeChangeDelegate.AddDynamic(this, &AMFG_LevelMusic::ChangeLevelMusic);
	}
}

void AMFG_LevelMusic::StopLevelMusic()
{
	MusicAudioComponent->Stop();
}

void AMFG_LevelMusic::ChangeLevelMusic(bool bIsAlert)
{
	MusicAudioComponent->SetBoolParameter(AlertParamName, bIsAlert);
}

