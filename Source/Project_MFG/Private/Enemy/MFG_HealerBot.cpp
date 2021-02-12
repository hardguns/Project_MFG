// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MFG_HealerBot.h"
#include "Components/StaticMeshComponent.h"
#include "Components/MFG_HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavigationPath.h"
#include "Enemy/MFG_Enemy.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "Objects/MFG_Shield.h"
#include "Components/CapsuleComponent.h"
#include "Project_MFG/Project_MFG.h"
#include "Weapons/MFG_Projectile.h"
#include "Components/PrimitiveComponent.h"

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
	DecalEffect->SetHiddenInGame(true);

	MinDistanceToTarget = 100.0f;
	ForceMagnitude = 500.0f;
	SecondsToHeal = 0.5f;
	HealAmount = 5.0f;
	ActionRadius = 400.0f;
	HealCounter = 0;
	ShieldSocketName = "SCK_Shield";

	HealDetectorComponent->SetSphereRadius(ActionRadius);
	ActorsEnum.Add(EObjectTypeQuery::ObjectTypeQuery1);
	ActorsEnum.Add(EObjectTypeQuery::ObjectTypeQuery3);
	ActorsEnum.Add(EObjectTypeQuery::ObjectTypeQuery7);
}

// Called when the game starts or when spawned
void AMFG_HealerBot::BeginPlay()
{
	Super::BeginPlay();

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (IsValid(PlayerPawn))
	{
		PlayerReference = Cast<AMFG_Character>(PlayerPawn);
	}

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

	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &AMFG_HealerBot::TakingDamage);
	HealDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &AMFG_HealerBot::StartAllyOverlap);
	HealDetectorComponent->OnComponentEndOverlap.AddDynamic(this, &AMFG_HealerBot::EndAllyOverlap);

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

		GetWorldTimerManager().ClearTimer(TimerHandle_BeginAttackBehaviour);
		Destroy();
	}
}

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
			if (CastedEnemy->HealthComponent->GetCurrentHealth() < CastedEnemy->HealthComponent->GetMaxHealth() && !CastedEnemy->HealthComponent->IsDead())
			{
				EnemyReference = CastedEnemy;
				bHasToHeal = true;
				break;
			}
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
		EnemyReference = NULL;
		bHasToHeal = false;
	}
	//New enemy appears on world
	else
	{
		bIsAttacking = false;
		GetWorldTimerManager().ClearTimer(TimerHandle_BeginAttackBehaviour);
	}

	//If EnemyReference is not valid it means that there are not more enemies on world, bot starts attack to player
	if (!IsValid(EnemyReference))
	{
		if (!bIsAttacking && !bHasToHeal)
		{
			AttackPlayer();
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_BeginAttackBehaviour, this, &AMFG_HealerBot::AttackPlayer, 1.0f, true);
		}

		return GetActorLocation();
	}

	UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), GetActorLocation(), EnemyReference);
	if (NavigationPath->PathPoints.Num() > 1)
	{
		return NavigationPath->PathPoints[1];
	}

	//If Navigation points are less or equal than 1
	return GetActorLocation();
}

void AMFG_HealerBot::StartAllyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(EnemyReference) && OtherActor == EnemyReference)
	{
		bIsOverlapping = true;
		if (EnemyReference->HealthComponent->GetCurrentHealth() == EnemyReference->HealthComponent->GetMaxHealth())
		{
			GiveShieldToAllies();
		}
	}
}

void AMFG_HealerBot::EndAllyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bIsOverlapping = false;

	UnsetBotBehaviour();
}

void AMFG_HealerBot::SetBotBehaviour()
{
	//Starts healing action
	if (!bIsHealing && bHasToHeal && bIsOverlapping)
	{
		bIsHealing = true;
		HealAllies();
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_HealTimer, this, &AMFG_HealerBot::HealAllies, SecondsToHeal, true);
	}
}

void AMFG_HealerBot::UnsetBotBehaviour()
{
	if (IsValid(HealingAttachedEffectComponent))
	{
		HealingAttachedEffectComponent->DestroyComponent();
	}

	if (IsValid(DecalEffect))
	{
		DecalEffect->SetHiddenInGame(true);
	}
}

void AMFG_HealerBot::HealAllies()
{
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), ActionRadius, ActorsEnum, nullptr, TArray<AActor*>(), ActorsToHeal);

	for (AActor* AllyToHeal : ActorsToHeal)
	{
		AMFG_Enemy* EnemyToHeal = Cast<AMFG_Enemy>(AllyToHeal);
		if (IsValid(EnemyToHeal))
		{
			EnemyToHeal->HealthComponent->SetNewHealth(HealAmount);

			if (bDebug)
			{
				DrawDebugSphere(GetWorld(), GetActorLocation(), ActionRadius, 15, FColor::Green, false, 5.0f, 0, 1.0f);
				UE_LOG(LogTemp, Log, TEXT("Current enemy health: %s"), *FString::SanitizeFloat(EnemyToHeal->HealthComponent->GetCurrentHealth()));
			}
		}
	}

	if (IsValid(DecalEffect))
	{
		DecalEffect->SetHiddenInGame(false);
		if (IsValid(DecalMaterial))
		{
			DecalMaterial->SetVectorParameterValue(FName("CircleColor"), FLinearColor::Green);
		}
	}

	if (IsValid(HealingEffect))
	{
		if (!IsValid(HealingAttachedEffectComponent))
		{
			HealingAttachedEffectComponent = UGameplayStatics::SpawnEmitterAttached(HealingEffect, BotMeshComponent);
		}
	}

	HealCounter += 0.5f;

	if (HealCounter >= 2.0f)
	{
		bIsHealing = false;
		bHasToHeal = false;
		HealCounter = 0;

		GetWorldTimerManager().ClearTimer(TimerHandle_HealTimer);

		if (IsValid(HealingAttachedEffectComponent))
		{
			HealingAttachedEffectComponent->DestroyComponent();
		}
	}
}

void AMFG_HealerBot::GiveShieldToAllies()
{
	if (IsValid(DecalEffect))
	{
		DecalEffect->SetHiddenInGame(false);
		if (IsValid(DecalMaterial))
		{
			DecalMaterial->SetVectorParameterValue(FName("CircleColor"), FLinearColor::Blue);
		}
	}

	FVector ShieldSocketLocation = EnemyReference->GetMesh()->GetSocketLocation(ShieldSocketName);

	if (IsValid(InitialShieldClass))
	{
		AMFG_Character* EnemyToCharacter = Cast<AMFG_Character>(EnemyReference);
		if (IsValid(EnemyToCharacter) && !IsValid(EnemyToCharacter->GetShieldActor()))
		{
			AMFG_Shield* NewShield = GetWorld()->SpawnActor<AMFG_Shield>(InitialShieldClass, ShieldSocketLocation, EnemyReference->GetActorRotation());

			NewShield->SetShieldOwner(EnemyReference);
			NewShield->HitCapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
			NewShield->HitCapsuleComponent->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Block);
			NewShield->AttachToComponent(EnemyReference->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ShieldSocketName);
		}
	}
}

void AMFG_HealerBot::AttackPlayer()
{
	bIsAttacking = true;

	if (IsValid(DecalEffect))
	{
		DecalEffect->SetHiddenInGame(true);
	}

	if (IsValid(PlayerReference))
	{
		if (IsValid(HealingAttachedEffectComponent))
		{
			HealingAttachedEffectComponent->DestroyComponent();
		}
		
		if (IsValid(BotMeshComponent))
		{
			FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerReference->GetActorLocation());
			SetActorRotation(NewRotation);
			FVector ShotLocation = (UKismetMathLibrary::GetForwardVector(GetControlRotation()) * 70) + GetActorLocation();
			AMFG_Projectile* CurrentProjectile = GetWorld()->SpawnActor<AMFG_Projectile>(ProjectileClass, ShotLocation, NewRotation);
		}
	}
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

	SetBotBehaviour();

	DrawDebugSphere(GetWorld(), NextPathPoint, 30.0f, 15, FColor::Purple, false, 0, 0, 1.0f);
}

