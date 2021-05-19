// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/MFG_HUDMessage.h"
#include "Animation/WidgetAnimation.h"

void UMFG_HUDMessage::InitializeWidget()
{
	HideMessageTime = 3.0f;

	ShowWidgetMessage();
}

void UMFG_HUDMessage::ShowWidgetMessage()
{
	SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_HideMessage, this, &UMFG_HUDMessage::HideWidgetMessage, HideMessageTime, false);
}

void UMFG_HUDMessage::HideWidgetMessage()
{
	PlayAnimation(HideMessageAnim);
}
