// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Enemy/MFG_EnemyHealthBar.h"

void UMFG_EnemyHealthBar::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	HealthPercentage = CurrentHealth / MaxHealth;
}
