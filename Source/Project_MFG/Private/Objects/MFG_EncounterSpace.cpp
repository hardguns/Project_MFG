// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/MFG_EncounterSpace.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/MFG_Enemy.h"
#include "AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"

// Sets default values
AMFG_EncounterSpace::AMFG_EncounterSpace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EncounterCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("EncounterCollider"));
	RootComponent = EncounterCollider;

	TargetLocationParameterName = "TargetLocation";

}

// Called when the game starts or when spawned
void AMFG_EncounterSpace::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMFG_EncounterSpace::CalloutTarget(FVector TargetLocation)
{
	PossiblePlayerLocation = TargetLocation;
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMFG_Enemy::StaticClass(), Enemies);

	for (AActor* PossibleEnemy : Enemies)
	{
		AMFG_Enemy* Enemy = Cast<AMFG_Enemy>(PossibleEnemy);
		if (IsValid(Enemy))
		{
			bool bIsEnemyOverlapping = IsOverlappingActor(Enemy);

			if (bIsEnemyOverlapping)
			{
				UBlackboardComponent* EnemyBlackboard = UAIBlueprintHelperLibrary::GetBlackboard(Enemy);

				if (IsValid(EnemyBlackboard))
				{
					EnemyBlackboard->SetValueAsVector(TargetLocationParameterName, TargetLocation);
				}
			}
		}
	}

	BP_CalloutTarget();
}

