// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MFG_Enemy.h"
#include "MFG_Character.h"
#include "Weapons/MFG_Weapon.h"
#include "Components/MFG_HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Items/MFG_Item.h"
#include "Enemy/Controller/MFG_AIController.h"
#include "AIModule/Classes/Perception/AISense_Damage.h"

AMFG_Enemy::AMFG_Enemy()
{
	bLoopPath = false;
	DirectionIndex = 1;
	WaitingTimeOnPathPoint = 1.0f;
	XPValue = 20.0f;

	LootProbability = 100.0f;
}

void AMFG_Enemy::BeginPlay()
{
	Super::BeginPlay();

	MyAIController = Cast<AMFG_AIController>(GetController());

	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &AMFG_Enemy::HealthChanged);
	HealthComponent->OnDeadDelegate.AddDynamic(this, &AMFG_Enemy::GiveXP);
}

void AMFG_Enemy::GiveXP(AActor* DamageCauser)
{
	AMFG_Character* PossiblePlayer = Cast<AMFG_Character>(DamageCauser);

	if (IsValid(PossiblePlayer) && PossiblePlayer->GetCharacterType() == EMFG_CharacterType::CharacterType_Player)
	{
		PossiblePlayer->GainUltimateXP(XPValue);
		TrySpawnLoot();
	}

	AMFG_Weapon* PossibleWeapon = Cast<AMFG_Weapon>(DamageCauser);

	if (IsValid(PossibleWeapon))
	{
		AMFG_Character* WeaponOwner = Cast<AMFG_Character>(PossibleWeapon->GetOwner());

		if (IsValid(WeaponOwner) && WeaponOwner->GetCharacterType() == EMFG_CharacterType::CharacterType_Player)
		{
			WeaponOwner->GainUltimateXP(XPValue);
			TrySpawnLoot();
		}
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BP_GiveXP(DamageCauser);
}

bool AMFG_Enemy::TrySpawnLoot()
{
	if (!IsValid(LootItemClass))
	{
		return false;
	}

	float SelectedProbability = FMath::RandRange(0.0f, 100.0f);
	if (SelectedProbability <= LootProbability)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<AMFG_Item>(LootItemClass, GetActorLocation(), FRotator::ZeroRotator, SpawnParameters);
	}

	return true;
}

void AMFG_Enemy::HealthChanged(UMFG_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!IsValid(MyAIController))
	{
		return;
	}
	if (CurrentHealthComponent->IsDead())
	{
		MyAIController->UnPossess();
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_DestroyEnemy, this, &AMFG_Enemy::DestroyEnemy, 4.0f, false);
	}
	else
	{
		AMFG_Weapon* Weapon = Cast<AMFG_Weapon>(DamageCauser);
		if (IsValid(Weapon))
		{
			AActor* WeaponOwner = Weapon->GetOwner();
			MyAIController->SetReceiveDamage(true);
			UAISense_Damage::ReportDamageEvent(GetWorld(), this, WeaponOwner, Damage, WeaponOwner->GetActorLocation(), FVector::ZeroVector);
		}
	}
}

void AMFG_Enemy::DestroyEnemy()
{
	Destroy();
}
