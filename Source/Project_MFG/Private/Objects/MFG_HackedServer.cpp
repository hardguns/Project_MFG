// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/MFG_HackedServer.h"
#include "Enemy/MFG_EnemySpawner.h"
#include "Components/MFG_HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/Enemy/MFG_EnemyHealthBar.h"
#include "Kismet/GameplayStatics.h"
#include "Core/MFG_GameMode.h"
#include "MFG_Character.h"
#include "MFG_Door.h"

AMFG_HackedServer::AMFG_HackedServer()
{
	HealthComponent = CreateDefaultSubobject<UMFG_HealthComponent>(TEXT("HealthComponent"));

	WidgetHealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetHealthBarComponent"));
	WidgetHealthBarComponent->SetupAttachment(RootComponent);
}

void AMFG_HackedServer::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &AMFG_HackedServer::OnHealthChange);

	//Gets widget object to subscribe to delegate
	UUserWidget* WidgetObject = WidgetHealthBarComponent->GetUserWidgetObject();
	if (IsValid(WidgetObject))
	{
		ServerHealthBar = Cast<UMFG_EnemyHealthBar>(WidgetObject);
		if (IsValid(ServerHealthBar))
		{
			HealthComponent->OnHealthUpdateDelegate.AddDynamic(ServerHealthBar, &UMFG_EnemyHealthBar::UpdateHealth);
			HideHealthBar();
		}
	}
}

void AMFG_HackedServer::CheckActivable(AMFG_Character* OtherActor)
{
	Super::CheckActivable(OtherActor);
}

void AMFG_HackedServer::UseActivable()
{
	if (bCanBeActivatedOnce && bWasActivated)
	{
		return;
	}

	//Iterate ActivableObjects array to activate enemySpawners and close doors
	for (AActor* Activable : ActivableObjects)
	{
		AMFG_Door* Door = Cast<AMFG_Door>(Activable);
		if (IsValid(Door))
		{
			Door->SetCanOpenDoor(false);
			Door->CloseDoor();
		}

		AMFG_EnemySpawner* EnemySpawner = Cast<AMFG_EnemySpawner>(Activable);
		if (IsValid(EnemySpawner))
		{
			EnemySpawner->SetIsActive(true);
		}
	}

	bWasActivated = true;

	//Broadcast delegate to notify HUD server has started to be hacked
	OnStartHackingDelegate.Broadcast(HackingTime);

	//Timer set to finish hacking after HackingTime completes
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_DownloadInformation, this, &AMFG_HackedServer::FinishHacking, 0.1f, false, HackingTime);
}

void AMFG_HackedServer::OnHealthChange(UMFG_HealthComponent* ServerHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	//Checks if actor needs to show HealthBar
	if (bIsShowingHealthBar)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_HideHealthBar);
	}
	else
	{
		ShowHealthBar();
	}

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_HideHealthBar, this, &AMFG_HackedServer::HideHealthBar, 1.0f, false);

	//Set game over state if server is destroyed by enemies
	if (ServerHealthComponent->IsDead())
	{
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (IsValid(PlayerPawn))
		{
			AMFG_Character* PlayerCharacter = Cast<AMFG_Character>(PlayerPawn);
			if (IsValid(PlayerCharacter))
			{
				GameModeReference->GameOver(PlayerCharacter);
			}
		}
		
	}
}

void AMFG_HackedServer::FinishHacking()
{
	//Disables collision to avoid damage after hacked
	ActivatorMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//Iterate ActivableObjects array to deactivate enemySpawners and open doors
	for (AActor* Activable : ActivableObjects)
	{
		//Looks for an Interactive object or a bot spawner to activate or deactivated;
		AMFG_Door* Door = Cast<AMFG_Door>(Activable);
		if (IsValid(Door))
		{
			Door->SetCanOpenDoor(true);
			if (Door->GetIsDoorOpen())
			{
				Door->OpenDoor();
			}
		}

		//Looks for an enemy spawner to deactivate it
		AMFG_EnemySpawner* EnemySpawner = Cast<AMFG_EnemySpawner>(Activable);
		if (IsValid(EnemySpawner))
		{
			EnemySpawner->SetIsActive(false);
		}
	}

	bWasHacked = true;
}

void AMFG_HackedServer::ShowHealthBar()
{
	bIsShowingHealthBar = true;
	ServerHealthBar->SetVisibility(ESlateVisibility::Visible);
}

void AMFG_HackedServer::HideHealthBar()
{
	bIsShowingHealthBar = false;
	ServerHealthBar->SetVisibility(ESlateVisibility::Hidden);
}
