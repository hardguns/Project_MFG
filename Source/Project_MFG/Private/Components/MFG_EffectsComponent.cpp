// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MFG_EffectsComponent.h"
#include "Objects/MFG_ExplosiveBarrel.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UMFG_EffectsComponent::UMFG_EffectsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	DamageSecondCounter = 0;
	TimeBetweenDamage = 1.0f;

}


// Called when the game starts
void UMFG_EffectsComponent::BeginPlay()
{
	Super::BeginPlay();

	MyOwner = GetOwner();

	if (IsValid(MyOwner))
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UMFG_EffectsComponent::ApplyBurningDamage);
	}

}

void UMFG_EffectsComponent::ApplyBurningDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	AMFG_ExplosiveBarrel* Barrel = Cast<AMFG_ExplosiveBarrel>(DamageCauser);
	if (IsValid(Barrel))
	{
		if (IsValid(DamagedActor))
		{
			bIsBurning = true;
			DamageSecondCounter = 0;

			TimerDelegate.BindUFunction(this, FName("MakeDamage"), DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);

			GetWorld()->GetTimerManager().SetTimer(TimerHandle_HandleEffects, TimerDelegate, TimeBetweenDamage, true);
		}
	}
}

void UMFG_EffectsComponent::MakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	DamageSecondCounter += 1;
	if (DamageSecondCounter <= 5 && bIsBurning)
	{
		float BurningDamage = FMath::RoundToFloat(Damage / 10.0f);
		UGameplayStatics::ApplyDamage(DamagedActor, BurningDamage, InstigatedBy, DamageCauser, DamageDealType);
		OnBurningStateChangeDelegate.Broadcast(this, DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
	}
	else 
	{
		if (bIsBurning)
		{
			bIsBurning = false;
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_HandleEffects);
			OnBurningStateChangeDelegate.Broadcast(this, DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
		}
	}
}


