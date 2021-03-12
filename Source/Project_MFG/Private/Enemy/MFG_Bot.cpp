// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MFG_Bot.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MFG_Character.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "DrawDebugHelpers.h"
#include "Components/MFG_HealthComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystem.h"
#include "Weapons/MFG_Weapon.h"
#include "Items/MFG_Item.h"
#include "Items/MFG_DoorKey.h"
#include "Enemy/MFG_BotSpawner.h"
#include "Core/MFG_GameInstance.h"

// Sets default values
AMFG_Bot::AMFG_Bot()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BotMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BotMeshComponent"));
	BotMeshComponent->SetCanEverAffectNavigation(false);
	BotMeshComponent->SetSimulatePhysics(true);
	RootComponent = BotMeshComponent;

	HealthComponent = CreateDefaultSubobject<UMFG_HealthComponent>(TEXT("HealthComponent"));

	SelfDestructionDetectorComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SelfDestructionDetector"));
	SelfDestructionDetectorComponent->SetupAttachment(RootComponent);
	SelfDestructionDetectorComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SelfDestructionDetectorComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SelfDestructionDetectorComponent->SetSphereRadius(150.0f);

	MinDistanceToTarget = 100.0f;
	ForceMagnitude = 500.0f;
	ExplosionDamage = 100.0f;
	ExplosionRadius = 50.0f;
	XPValue = 10.0f;

	LootProbability = 100.0f;
}

// Called when the game starts or when spawned
void AMFG_Bot::BeginPlay()
{
	Super::BeginPlay();

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (IsValid(PlayerPawn))
	{
		PlayerCharacter = Cast<AMFG_Character>(PlayerPawn);
	}

	GameInstanceReference = Cast<UMFG_GameInstance>(GetWorld()->GetGameInstance());

	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &AMFG_Bot::TakingDamage);
	HealthComponent->OnDeadDelegate.AddDynamic(this, &AMFG_Bot::GiveXP);

	SelfDestructionDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &AMFG_Bot::StartCountDown);

	BotMaterial = BotMeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, BotMeshComponent->GetMaterial(0));

	NextPathPoint = GetNextPathPoint();
}

FVector AMFG_Bot::GetNextPathPoint()
{
	if (!IsValid(PlayerCharacter))
	{
		return GetActorLocation();
	}

	UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), GetActorLocation(), PlayerCharacter);
	if (NavigationPath->PathPoints.Num() > 1)
	{
		return NavigationPath->PathPoints[1];
	}

	//If Navigation points are less or equal than 1
	return GetActorLocation();
}

void AMFG_Bot::TakingDamage(UMFG_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (IsValid(BotMaterial))
	{
		BotMaterial->SetScalarParameterValue("Pulse", GetWorld()->TimeSeconds);
	}

	if (CurrentHealthComponent->IsDead())
	{
		if (IsValid(DamageCauser))
		{
			AMFG_Weapon* Weapon = Cast<AMFG_Weapon>(DamageCauser);
			if (IsValid(Weapon))
			{
				AMFG_Character* RifleOwner = Cast<AMFG_Character>(Weapon->GetOwner());
				if (IsValid(RifleOwner) && RifleOwner->GetCharacterType() == EMFG_CharacterType::CharacterType_Player)
				{
					TrySpawnLoot();

					if (IsValid(GameInstanceReference))
					{
						GameInstanceReference->AddEnemyDefeatedToCounter();
					}
				}
			}
		}

		SelfDestruction();
	}
}

void AMFG_Bot::SelfDestruction()
{
	if (bHasExploded)
	{
		return;
	}

	bHasExploded = true;

	if (IsValid(ExplosionEffect))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
	}

	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);

	UGameplayStatics::ApplyRadialDamage(GetWorld(), ExplosionDamage, GetActorLocation(), ExplosionRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true);

	if (bDebug)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 20, FColor::Red, true, 5.0f, 0, 2.0f);
	}

	if (IsValid(MySpawner))
	{
		MySpawner->NotifyBotDead();
	}

	Destroy();
}

void AMFG_Bot::StartCountDown(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsStartingCountDown)
	{
		return;
	}

	if (OtherActor == PlayerCharacter)
	{
		bIsStartingCountDown = true;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_SelfDamage, this, &AMFG_Bot::SelfDamage, 0.5f, true);
	}
}

void AMFG_Bot::SelfDamage()
{
	UGameplayStatics::ApplyDamage(this, 20.0f, GetInstigatorController(), nullptr, nullptr);
}

void AMFG_Bot::GiveXP(AActor* DamageCauser)
{
	AMFG_Character* PossiblePlayer = Cast<AMFG_Character>(DamageCauser);

	if (IsValid(PossiblePlayer) && PossiblePlayer->GetCharacterType() == EMFG_CharacterType::CharacterType_Player)
	{
		PossiblePlayer->GainUltimateXP(XPValue);
	}

	AMFG_Weapon* PossibleWeapon = Cast<AMFG_Weapon>(DamageCauser);

	if (IsValid(PossibleWeapon))
	{
		AMFG_Character* WeaponOwner = Cast<AMFG_Character>(PossibleWeapon->GetOwner());

		if (IsValid(WeaponOwner) && WeaponOwner->GetCharacterType() == EMFG_CharacterType::CharacterType_Player)
		{
			WeaponOwner->GainUltimateXP(XPValue);
		}
	}

	BP_GiveXP(DamageCauser);
}

bool AMFG_Bot::TrySpawnLoot()
{
	if (LootItemsClass.Num() == 0)
	{
		return false;
	}

	float SelectedProbability = FMath::RandRange(0.0f, 100.0f);
	float CurrentLootProbability = 0;
	int SelectedItem = FMath::RandRange(0, LootItemsClass.Num() - 1);

	if (LootProbabilities.Num() > 0)
	{
		CurrentLootProbability = LootProbabilities[SelectedItem];
	}
	else 
	{
		CurrentLootProbability = LootProbability;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (!MySpawner->GetBotSpawnerState() && MySpawner->GetCurrentBotsOnScene() == 1 && !PlayerCharacter->HasKey(MySpawner->GetKeyTagToSpawn()))
	{
		if (IsValid(LastLootItemClass))
		{
			//FVector SpawnPoint = UKismetMathLibrary::TransformLocation(GetActorTransform(), );
			FTransform ItemTransform = FTransform(FRotator::ZeroRotator, GetActorLocation());
			AMFG_Item* NewItem = GetWorld()->SpawnActorDeferred<AMFG_Item>(LastLootItemClass, ItemTransform);

			AMFG_DoorKey* PossibleDoorKey = Cast<AMFG_DoorKey>(NewItem);
			if (IsValid(PossibleDoorKey))
			{
				//if (!MySpawner->GetKeyTagToSpawn().IsNone)
				//{
					PossibleDoorKey->SetKeyTag(MySpawner->GetKeyTagToSpawn());
				//}

			}
			NewItem->FinishSpawning(ItemTransform);
		}
	}
	else if (SelectedProbability <= CurrentLootProbability)
	{
		GetWorld()->SpawnActor<AMFG_Item>(LootItemsClass[SelectedItem], GetActorLocation(), FRotator::ZeroRotator, SpawnParameters);
	}

	return true;
}

// Called every frame
void AMFG_Bot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();

	if (DistanceToTarget <= MinDistanceToTarget)
	{
		NextPathPoint = GetNextPathPoint();
	}
	else
	{
		FVector ForceDirection = NextPathPoint - GetActorLocation();
		ForceDirection.Normalize();
		ForceDirection *= ForceMagnitude;

		BotMeshComponent->AddForce(ForceDirection, NAME_None, true);
	}

	if (bDebug)
	{
		DrawDebugSphere(GetWorld(), NextPathPoint, 30.0f, 15, FColor::Purple, false, 0, 0, 1.0f);
	}
}
