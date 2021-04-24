// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MFG_EnemySpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BillboardComponent.h"
#include "Enemy/MFG_Enemy.h"

// Sets default values
AMFG_EnemySpawner::AMFG_EnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnerBillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("BillboardComponent"));
	RootComponent = SpawnerBillboardComponent;

	bIsActive = true;
	MaxEnemyCounter = 1;
	TimeToSpawn = 1.0f;
	MaxEnemyHordes = 1;
	CurrentEnemyHordes = 1;

	DirectionIndexes.Add(-1);
	DirectionIndexes.Add(1);
}

// Called when the game starts or when spawned
void AMFG_EnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_SpawnEnemy, this, &AMFG_EnemySpawner::SpawnEnemy, TimeToSpawn, true);
}

FVector AMFG_EnemySpawner::GetSpawnPoint()
{
	if (SpawnPoints.Num() > 0)
	{
		int SpawnPointIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
		return SpawnPoints[SpawnPointIndex];
	}
	else
	{
		return GetActorLocation();
	}
}

void AMFG_EnemySpawner::SpawnEnemy()
{
	if (!bIsActive)
	{
		return;
	}

	if (bHasToSpawnEnemyHordes)
	{
		if (CurrentHordeEnemyCounter >= MaxEnemyCounter)
		{
			return;
		}

		if (CurrentEnemyHordes > MaxEnemyHordes)
		{
			return;
		}
	}
	else
	{
		if (CurrentEnemyCounter >= MaxEnemyCounter)
		{
			return;
		}
	}

	if (EnemyClasses.Num() == 0)
	{
		return;
	}

	int indexSelected = FMath::RandRange(0, EnemyClasses.Num() - 1);

	if (IsValid(EnemyClasses[indexSelected]))
	{
		FVector LocalSpawnPoint = GetSpawnPoint();
		FVector SpawnPoint = UKismetMathLibrary::TransformLocation(GetActorTransform(), LocalSpawnPoint);
		FActorSpawnParameters SpawnParameters;
		FTransform EnemyTransform = FTransform(FRotator::ZeroRotator, SpawnPoint);

		AMFG_Enemy* NewEnemy = GetWorld()->SpawnActorDeferred<AMFG_Enemy>(EnemyClasses[indexSelected], EnemyTransform);
		//AMFG_Enemy* NewEnemy = GetWorld()->SpawnActor<AMFG_Enemy>(EnemyClasses[indexSelected], SpawnPoint, FRotator::ZeroRotator, SpawnParameters);
		if (IsValid(NewEnemy))
		{
			if (DirectionIndexes.Num() > 0)
			{
				int selectedIndex = FMath::RandRange(0, 1);

				NewEnemy->SetDirectionIndex(DirectionIndexes[selectedIndex]);
			}
			else
			{
				NewEnemy->SetDirectionIndex(1);
			}
			
			NewEnemy->SetEnemySpawner(this);

			//Set Path Actor for patrolling
			if (EnemyActorPaths.Num() > 0)
			{
				int pathIndexSelected = FMath::RandRange(0, EnemyActorPaths.Num() - 1);
				NewEnemy->SetPathActor(EnemyActorPaths[pathIndexSelected]);
			}

			NewEnemy->FinishSpawning(EnemyTransform);

			CurrentEnemyCounter++;
			if (bHasToSpawnEnemyHordes)
			{
				CurrentHordeEnemyCounter++;
			}
		}
	}
}

void AMFG_EnemySpawner::NotifyEnemyDead()
{
	CurrentEnemyCounter--;

	if (CurrentEnemyCounter == 0)
	{
		NotifyEnemiesDefeated();
	}
}

void AMFG_EnemySpawner::NotifyEnemiesDefeated()
{
	if (bHasToSpawnEnemyHordes)
	{
		CurrentEnemyHordes++;
		CurrentHordeEnemyCounter = 0;
		if (CurrentEnemyHordes > MaxEnemyHordes)
		{
			bIsActive = false;
			OnEnemiesSpawnedDefeatedDelegate.Broadcast();
		}
	}
	else
	{
		if (bHasToStopSpawning)
		{
			bIsActive = false;
			OnEnemiesSpawnedDefeatedDelegate.Broadcast();
		}
	}
}

