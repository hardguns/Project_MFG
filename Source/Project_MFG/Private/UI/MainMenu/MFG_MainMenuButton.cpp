// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenu/MFG_MainMenuButton.h"

UMFG_MainMenuButton::UMFG_MainMenuButton()
{
	CheckFocusRate = 0.1f;
}

void UMFG_MainMenuButton::Start()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CheckFocus, this, &UMFG_MainMenuButton::CheckFocus, CheckFocusRate, true);
}

void UMFG_MainMenuButton::CheckFocus()
{
	if (IsHovered())
	{
		//This button is affected by mouse 
		SetKeyboardFocus();
		SetButtonStyle(EMFG_ButtonStyleType::ButtonStyleType_Mouse);
	}
	else
	{
		//Check if the keyboard/gamepad is on me!
		EMFG_ButtonStyleType StyleTypeSelected = HasKeyboardFocus() ? EMFG_ButtonStyleType::ButtonStyleType_KeySelected : EMFG_ButtonStyleType::ButtonStyleType_KeyNotSelected;
		SetButtonStyle(StyleTypeSelected);
	}
}

void UMFG_MainMenuButton::SetButtonStyle(EMFG_ButtonStyleType NewStyleType)
{
	BP_SetButtonStyle(NewStyleType);
}
