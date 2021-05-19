// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/MFG_HUDServerHacking.h"
#include "Objects/MFG_HackedServer.h"
#include "Kismet/GameplayStatics.h"

void UMFG_HUDServerHacking::InitializeWidget()
{
	TArray<AActor*> HackedServers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMFG_HackedServer::StaticClass(), HackedServers);

	for (AActor* ServerActor : HackedServers)
	{
		AMFG_HackedServer* HackedServer = Cast<AMFG_HackedServer>(ServerActor);
		if (IsValid(HackedServer))
		{
			HackedServer->OnStartHackingDelegate.AddDynamic(this, &UMFG_HUDServerHacking::StartHacking);
		}
	}
}

void UMFG_HUDServerHacking::ShowWidget()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UMFG_HUDServerHacking::HideWidget()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UMFG_HUDServerHacking::StartHacking(float HackingTime)
{
	if (!bIsActive)
	{
		TotalHackingTime = HackingTime;
		ShowWidget();

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_HackingTime, this, &UMFG_HUDServerHacking::FillBar, true, 1.0f);
		bIsActive = true;
	}
}

void UMFG_HUDServerHacking::FillBar()
{
	if (HackingPercentage <= 1.0f)
	{
		CurrentHackingTime++;
		HackingPercentage = CurrentHackingTime / TotalHackingTime;
	}
	else
	{
		HideWidget();
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_HackingTime);
	}
}
