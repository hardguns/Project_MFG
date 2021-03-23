// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/MFG_HUDHealthBar.h"
#include "Kismet/GameplayStatics.h"
#include "MFG_Character.h"
#include "Components/MFG_HealthComponent.h"
#include "Animation/WidgetAnimation.h"

void UMFG_HUDHealthBar::InitializeWidget()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (IsValid(PlayerPawn))
	{
		AMFG_Character* PlayerCharacter = Cast<AMFG_Character>(PlayerPawn);
		if (IsValid(PlayerCharacter))
		{
			UMFG_HealthComponent* PlayerHealthComponent = PlayerCharacter->GetHealthComponent();
			if (IsValid(PlayerHealthComponent))
			{
				PlayerHealthComponent->OnHealthUpdateDelegate.AddDynamic(this, &UMFG_HUDHealthBar::UpdateHealth);
			}
		}
	}

}

void UMFG_HUDHealthBar::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	HealthPercentage = CurrentHealth / MaxHealth;
	HealthColor = FMath::Lerp(EmptyHealthColor, FullHealthColor, HealthPercentage);

	if (HealthPercentage <= 0.2f)
	{
		PlayAnimation(HealthBarFlashing, 0.0f, 0);
	}
	else
	{
		StopAnimation(HealthBarFlashing);
	}

	BP_UpdateHealth(CurrentHealth, MaxHealth);
}
