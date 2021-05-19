// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MFG_HealerBot.h"
#include "Enemy/MFG_Enemy.h"
#include "Components/StaticMeshComponent.h"
#include "Components/MFG_HealthComponent.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Objects/MFG_Shield.h"
#include "Project_MFG/Project_MFG.h"
#include "Weapons/MFG_Projectile.h"
#include "Weapons/MFG_Weapon.h"
#include "Items/MFG_Item.h"
#include "Core/MFG_GameInstance.h"
#include "Sound/SoundCue.h"

// Sets default values
AMFG_HealerBot::AMFG_HealerBot()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BotMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BotMeshComponent"));
	RootComponent = BotMeshComponent;

	HealthComponent = CreateDefaultSubobject<UMFG_HealthComponent>(TEXT("HealthComponent"));

	HealDetectorComponent = CreateDefaultSubobject<USphereComponent>(TEXT("HealDetectorComponent"));
	HealDetectorComponent->SetupAttachment(RootComponent);
	HealDetectorComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	HealDetectorComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

	DecalEffect = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalEffect->SetupAttachment(HealDetectorComponent);
	DecalEffect->SetVisibility(false);

	MinDistanceToTarget = 100.0f;
	ForceMagnitude = 500.0f;
	TimeBetweenHealing = 0.5f;
	MaxTimeHealing = 4.0f;
	HealAmount = 5.0f;
	ActionRadius = 400.0f;
	XPValue = 10.0f;
	ShieldSocketName = "SCK_Shield";

	LootProbability = 100.0f;

	HealDetectorComponent->SetSphereRadius(ActionRadius);
	ActorsEnum.Add(EObjectTypeQuery::ObjectTypeQuery1);
	ActorsEnum.Add(EObjectTypeQuery::ObjectTypeQuery3);
	ActorsEnum.Add(EObjectTypeQuery::ObjectTypeQuery7);
}

// Called when the game starts or when spawned
void AMFG_HealerBot::BeginPlay()
{
	Super::BeginPlay();

	//Gets player reference placed in world
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (IsValid(PlayerPawn))
	{
		PlayerReference = Cast<AMFG_Character>(PlayerPawn);
	}

	GameInstanceReference = Cast<UMFG_GameInstance>(GetWorld()->GetGameInstance());

	//Creates a decal effect to show how the bot is doing at the moment
	if (IsValid(DecalEffect))
	{
		DecalEffect->DecalSize.Y = ActionRadius;
		DecalEffect->DecalSize.Z = ActionRadius;
		DecalMaterial = UMaterialInstanceDynamic::Create(DecalEffect->GetMaterial(0), nullptr);
		if (IsValid(DecalMaterial))
		{
			DecalEffect->SetDecalMaterial(DecalMaterial);
		}
	}

	BotMaterial = BotMeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, BotMeshComponent->GetMaterial(0));

	//Delegates
	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &AMFG_HealerBot::TakingDamage);
	HealthComponent->OnDeadDelegate.AddDynamic(this, &AMFG_HealerBot::GiveXP);

	HealDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &AMFG_HealerBot::StartAllyOverlap);

	//Get initial path point
	NextPathPoint = GetNextPathPoint();
}

void AMFG_HealerBot::TakingDamage(UMFG_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (IsValid(BotMaterial))
	{
		BotMaterial->SetScalarParameterValue("Pulse", GetWorld()->TimeSeconds);
	}

	if (CurrentHealthComponent->IsDead())
	{
		if (IsValid(ExplosionEffect))
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
		}

		//Stop attacking to player when dead 
		if (bIsAttacking)
		{
			GetWorldTimerManager().ClearTimer(TimerHandle_BeginAttackBehaviour);
		}

		if (IsValid(GameInstanceReference))
		{
			//Sums value to EnemyCounter variable on Game Instance
			GameInstanceReference->AddEnemyDefeatedToCounter();
		}

		PlayExplosionSound();

		Destroy();
	}
}

//Gets bot's next path point to go to
FVector AMFG_HealerBot::GetNextPathPoint()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMFG_Enemy::StaticClass(), FoundActors);
	float ClosestDistance = 3000.0f;

	for (AActor* PossibleEnemy : FoundActors)
	{
		AMFG_Enemy* CastedEnemy = Cast<AMFG_Enemy>(PossibleEnemy);

		if (IsValid(CastedEnemy))
		{
			//Checks if current enemy needs to be healed
			if (CastedEnemy->HealthComponent->GetCurrentHealth() < CastedEnemy->HealthComponent->GetMaxHealth() && !CastedEnemy->HealthComponent->IsDead())
			{
				EnemyReference = CastedEnemy;
				bHasToHeal = true;
				break;
			}
			//Checks if current enemy is closer to the bot to give him a shield
			else if (!CastedEnemy->HealthComponent->IsDead())
			{
				float Distance = GetDistanceTo(CastedEnemy);
				if (Distance < ClosestDistance)
				{
					ClosestDistance = Distance;
					EnemyReference = CastedEnemy;
					bHasToHeal = false;
				}
			}
		}
	}

	//Sets EnemyReference to null to reset variable
	if (FoundActors.Num() == 0)
	{
		UnsetBotBehaviour();
		EnemyReference = nullptr;
		bHasToHeal = false;

		if (!bIsAttacking)
		{
			bIsAttacking = true;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_BeginAttackBehaviour, this, &AMFG_HealerBot::AttackPlayer, 1.0f, true);
		}

		return GetActorLocation();
	}
	//New enemy appears on world
	else
	{
		bIsAttacking = false;
		GetWorldTimerManager().ClearTimer(TimerHandle_BeginAttackBehaviour);
	}

	UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), GetActorLocation(), EnemyReference);
	if (NavigationPath->PathPoints.Num() > 1)
	{
		return NavigationPath->PathPoints[1];
	}

	//If Navigation points are less or equal than 1
	return GetActorLocation();
}

//Checks if bot is overlapping to current enemy reference in GetNextPathPoint method
void AMFG_HealerBot::StartAllyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		AMFG_Enemy* OverlappedEnemy = Cast<AMFG_Enemy>(OtherActor);
		if (IsValid(OverlappedEnemy))
		{
			SetBotBehaviour();
		}
	}
}

//Sets if bot has to heal or has to give shield to its "allies" 
void AMFG_HealerBot::SetBotBehaviour()
{
	//Starts healing action
	if (!bIsHealing && bHasToHeal)
	{
		bIsHealing = true;
		//HealAllies();
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_HealTimer, this, &AMFG_HealerBot::HealAllies, TimeBetweenHealing, true);
	}
	//Starts shielding action
	else if (!bIsHealing && !bHasToHeal && !bIsAttacking)
	{
		GiveShieldToAllies();
	}
}

//Unset any bot behaviour (healing or giving shield)
void AMFG_HealerBot::UnsetBotBehaviour()
{
	//Hides decal effect in order to make player know bot is not making any action
	if (IsValid(DecalEffect))
	{
		DecalEffect->SetVisibility(false);
	}

	//Hides healing effect if visible
	if (!IsValid(HealingAttachedEffectComponent))
	{
		return;
	}
	else
	{
		HealingAttachedEffectComponent->SetVisibility(false);
	}
}

void AMFG_HealerBot::HealAllies()
{
	//Look for actors in specific radius to get possible enemies to heal
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), ActionRadius, ActorsEnum, nullptr, TArray<AActor*>(), ActorsToHeal);

	for (AActor* AllyToHeal : ActorsToHeal)
	{
		AMFG_Enemy* EnemyToHeal = Cast<AMFG_Enemy>(AllyToHeal);
		if (IsValid(EnemyToHeal))
		{
			EnemyToHeal->TryAddHealth(HealAmount);

			if (bDebug)
			{
				DrawDebugSphere(GetWorld(), GetActorLocation(), ActionRadius, 15, FColor::Green, false, 5.0f, 0, 1.0f);
				UE_LOG(LogTemp, Log, TEXT("Current enemy health: %s"), *FString::SanitizeFloat(EnemyToHeal->HealthComponent->GetCurrentHealth()));
			}
		}
	}

	//Set decal to visibility and changes color
	if (IsValid(DecalEffect))
	{
		DecalEffect->SetVisibility(true);
		if (IsValid(DecalMaterial))
		{
			DecalMaterial->SetVectorParameterValue(FName("CircleColor"), FLinearColor::Green);
		}
	}

	//Set healing effect visibility and changes color
	if (IsValid(HealingEffect))
	{
		if (!IsValid(HealingAttachedEffectComponent))
		{
			HealingAttachedEffectComponent = UGameplayStatics::SpawnEmitterAttached(HealingEffect, BotMeshComponent);
		}
		else
		{
			HealingAttachedEffectComponent->SetVisibility(true);
		}
	}

	HealCounter++;

	//Heal will stop when heal counter is greater or equals than MaxTimeHealing variable
	TryStopHealing();
}

void AMFG_HealerBot::TryStopHealing()
{
	if (HealCounter >= MaxTimeHealing)
	{
		bIsHealing = false;
		bHasToHeal = false;
		HealCounter = 0;

		GetWorldTimerManager().ClearTimer(TimerHandle_HealTimer);

		UnsetBotBehaviour();
	}
}

void AMFG_HealerBot::GiveShieldToAllies()
{
	//Set decal to visibility and changes color
	if (IsValid(DecalEffect))
	{
		DecalEffect->SetVisibility(true);
		if (IsValid(DecalMaterial))
		{
			DecalMaterial->SetVectorParameterValue(FName("CircleColor"), FLinearColor::Blue);
		}
	}

	//Look for actors in specific radius to get possible enemies to give shield
	if (IsValid(InitialShieldClass))
	{
		TArray<AActor*> ActorsToGiveShield;
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), ActionRadius, ActorsEnum, nullptr, TArray<AActor*>(), ActorsToGiveShield);

		for (AActor* PossibleEnemy : ActorsToGiveShield)
		{
			AMFG_Character* EnemyToCharacter = Cast<AMFG_Character>(PossibleEnemy);
			if (IsValid(EnemyToCharacter) && EnemyToCharacter->GetCharacterType() == EMFG_CharacterType::CharacterType_Enemy && !IsValid(EnemyToCharacter->GetShieldActor()))
			{
				FVector ShieldSocketLocation = EnemyToCharacter->GetMesh()->GetSocketLocation(ShieldSocketName);
				AMFG_Shield* NewShield = GetWorld()->SpawnActor<AMFG_Shield>(InitialShieldClass, ShieldSocketLocation, EnemyToCharacter->GetActorRotation());

				NewShield->SetShieldOwner(EnemyToCharacter);
				NewShield->HitCapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
				NewShield->HitCapsuleComponent->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Block);
				NewShield->AttachToComponent(EnemyToCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ShieldSocketName);
			}
		}
	}

	UnsetBotBehaviour();
}

//If there are no enemies in map, bot will attack player
void AMFG_HealerBot::AttackPlayer()
{
	if (IsValid(PlayerReference))
	{
		if (IsValid(BotMeshComponent))
		{
			FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerReference->GetActorLocation());
			SetActorRotation(NewRotation);
			FVector ShotLocation = (UKismetMathLibrary::GetForwardVector(GetControlRotation()) * 75) + GetActorLocation();
			ShotLocation.Z += 20.0f;
			AMFG_Projectile* CurrentProjectile = GetWorld()->SpawnActor<AMFG_Projectile>(ProjectileClass, ShotLocation, NewRotation);
		}
	}
}

//Gives XP to player when killing bot
void AMFG_HealerBot::GiveXP(AActor* DamageCauser)
{
	//Checks if player or weapon have made damage to give XP and spawn loot
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

	BP_GiveXP(DamageCauser);
}

//Calculates probability in order to spawn or not loot as reward for player
bool AMFG_HealerBot::TrySpawnLoot()
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

// Called every frame
void AMFG_HealerBot::Tick(float DeltaTime)
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

void AMFG_HealerBot::PlayExplosionSound()
{
	if (!IsValid(ExplosionSound))
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
}

