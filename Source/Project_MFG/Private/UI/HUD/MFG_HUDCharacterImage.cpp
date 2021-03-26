// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/MFG_HUDCharacterImage.h"
#include "MFG_Character.h"
#include "Kismet/GameplayStatics.h"

void UMFG_HUDCharacterImage::Start()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_LoadWidget, this, &UMFG_HUDCharacterImage::InitializeWidget, StartDelayDuration, false);
}

void UMFG_HUDCharacterImage::InitializeWidget()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (IsValid(PlayerPawn))
	{
		AMFG_Character* PlayerCharacter = Cast<AMFG_Character>(PlayerPawn);

		if (IsValid(PlayerCharacter))
		{
			CharacterIcon = PlayerCharacter->GetCharacterHUDImage();
		}
	}
}
