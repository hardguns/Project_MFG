// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MFG_RadarComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MFG_Character.h"
#include "UI/HUD/MFG_HUD.h"
#include "UI/HUD/MFG_HUDRadar.h"

// Sets default values for this component's properties
UMFG_RadarComponent::UMFG_RadarComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMFG_RadarComponent::BeginPlay()
{
	Super::BeginPlay();

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (IsValid(PlayerPawn))
	{
		AMFG_Character* PlayerCharacter = Cast<AMFG_Character>(PlayerPawn);
		if (IsValid(PlayerCharacter))
		{
			UMFG_HUD* MainHUD = PlayerCharacter->GetCharacterMainHUD();
			if (IsValid(MainHUD))
			{
				MainHUD->W_Radar->AddToRadar(GetOwner());
			}
		}
	}
}

void UMFG_RadarComponent::EndPlay(EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (IsValid(PlayerPawn))
	{
		AMFG_Character* PlayerCharacter = Cast<AMFG_Character>(PlayerPawn);
		if (IsValid(PlayerCharacter))
		{
			UMFG_HUD* MainHUD = PlayerCharacter->GetCharacterMainHUD();
			if (IsValid(MainHUD))
			{
				MainHUD->W_Radar->RemoveFromRadar(GetOwner());
			}
		}
	}
}

// Called every frame
void UMFG_RadarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

