// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MFG_BotSpawner.h"
#include "Components/BillboardComponent.h"
#include "Enemy/MFG_Bot.h"
#include "Kismet/KismetMathLibrary.h"
#include "Items/MFG_Item.h"
#include "Items/MFG_DoorKey.h"

// Sets default values
AMFG_BotSpawner::AMFG_BotSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnerBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("SpanwerBillboard"));
	RootComponent = SpawnerBillboardComponent;

	bIsActive = true;
	MaxBotsCounter = 1;
	TimeToSpawn = 1.0f;
	KeyTag = "None";

	LootProbability = 100.0f;
}

// Called when the game starts or when spawned
void AMFG_BotSpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_SpawnBot, this, &AMFG_BotSpawner::SpawnBot, TimeToSpawn, true);
	
}

void AMFG_BotSpawner::SpawnBot()
{
	if (!bIsActive)
	{
		return;
	}

	if (CurrentBotsCounter >= MaxBotsCounter)
	{
		return;
	}

	if (IsValid(BotClass))
	{
		FVector LocalSpawnPoint = GetSpawnPoint();
		FVector SpawnPoint = UKismetMathLibrary::TransformLocation(GetActorTransform(), LocalSpawnPoint);
		FActorSpawnParameters SpawnParameters;
		FTransform BotTransform = FTransform(FRotator::ZeroRotator, SpawnPoint);

		AMFG_Bot* NewBot = GetWorld()->SpawnActorDeferred<AMFG_Bot>(BotClass, BotTransform);
		if (IsValid(NewBot))
		{
			NewBot->SetSpawner(this);
		}
		NewBot->FinishSpawning(BotTransform);

		CurrentBotsCounter++;
	}
}

FVector AMFG_BotSpawner::GetSpawnPoint()
{
	if (SpawnPoints.Num() > 0)
	{
		int indexSelected = FMath::RandRange(0, SpawnPoints.Num() - 1);
		return SpawnPoints[indexSelected];
	}
	else
	{
		return GetActorLocation();
	}
}

bool AMFG_BotSpawner::TrySpawnLoot()
{
	if (!IsValid(LootItemClass))
	{
		return false;
	}

	if (bHasSpawnedItem)
	{
		return false;
	}

	float SelectedProbability = FMath::RandRange(0.0f, 100.0f);

	if (SelectedProbability <= LootProbability && !bIsActive)
	{
		FTransform ItemTransform = FTransform(FRotator::ZeroRotator, GetActorLocation());
		AMFG_Item* NewItem = GetWorld()->SpawnActorDeferred<AMFG_Item>(LootItemClass, ItemTransform);

		AMFG_DoorKey* PossibleDoorKey = Cast<AMFG_DoorKey>(NewItem);
		if (IsValid(PossibleDoorKey))
		{
			PossibleDoorKey->SetKeyTag(KeyTag);
		}
		NewItem->FinishSpawning(ItemTransform);

		bHasSpawnedItem = true;
	}

	return true;
}

void AMFG_BotSpawner::NotifyBotDead()
{
	CurrentBotsCounter--;

	TrySpawnLoot();
}

