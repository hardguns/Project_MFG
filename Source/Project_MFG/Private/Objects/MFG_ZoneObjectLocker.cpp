// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/MFG_ZoneObjectLocker.h"
#include "Components/BoxComponent.h"
#include "Enemy/MFG_EnemySpawner.h"
#include "MFG_Character.h"
#include "MFG_Door.h"

// Sets default values
AMFG_ZoneObjectLocker::AMFG_ZoneObjectLocker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActivableZoneComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ActivableZoneComponent"));
	ActivableZoneComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	ActivableZoneComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = ActivableZoneComponent;

}

// Called when the game starts or when spawned
void AMFG_ZoneObjectLocker::BeginPlay()
{
	Super::BeginPlay();
	
	ActivableZoneComponent->OnComponentBeginOverlap.AddDynamic(this, &AMFG_ZoneObjectLocker::TriggerActivateAction);

	if (EnemySpawnersToTrigger.Num() > 0)
	{
		for (AMFG_EnemySpawner* EnemySpawner : EnemySpawnersToTrigger)
		{
			EnemySpawner->OnEnemiesSpawnedDefeatedDelegate.AddDynamic(this, &AMFG_ZoneObjectLocker::TriggerContinueAction);
		}
	}
}

void AMFG_ZoneObjectLocker::TriggerObjects()
{
	if (DoorsToTrigger.Num() > 0)
	{
		for (AMFG_Door* Door : DoorsToTrigger)
		{
			Door->SetCanOpenDoor(false);
			Door->CloseDoor();
		}
	}

	if (EnemySpawnersToTrigger.Num() > 0)
	{
		for (AMFG_EnemySpawner* EnemySpawner : EnemySpawnersToTrigger)
		{
			EnemySpawner->SetIsActive(true);
		}
	}

	bWasTriggered = true;
}

void AMFG_ZoneObjectLocker::TriggerContinueAction()
{
	if (EnemySpawnersToTrigger.Num() > 0)
	{
		for (AMFG_EnemySpawner* EnemySpawner : EnemySpawnersToTrigger)
		{
			if (EnemySpawner->GetIsActive())
			{
				return;
			}
		}
	}

	if (DoorsToTrigger.Num() > 0)
	{
		for (AMFG_Door* Door : DoorsToTrigger)
		{
			Door->SetCanOpenDoor(true);
			if (Door->GetIsDoorOpen())
			{
				Door->OpenDoor();
			}
		}
	}
}

void AMFG_ZoneObjectLocker::TriggerActivateAction(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bWasTriggered)
	{
		return;
	}

	if (IsValid(OtherActor))
	{
		AMFG_Character* PlayerCharacter = Cast<AMFG_Character>(OtherActor);
		if (IsValid(PlayerCharacter) && PlayerCharacter->GetCharacterType() == EMFG_CharacterType::CharacterType_Player)
		{
			TriggerObjects();
		}
	}
}

