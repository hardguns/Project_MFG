// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/MFG_AbilityFireball.h"
#include "Weapons/MFG_FireballProjectile.h"
#include "Gameframework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "MFG_Character.h"
#include "Enemy/MFG_Enemy.h"

AMFG_AbilityFireball::AMFG_AbilityFireball()
{
	TimeToShot = 2.5f;
	ProjectileInitialSpeed = 120.0f;
}

void AMFG_AbilityFireball::BeginPlay()
{
	Super::BeginPlay();

	AbilityDetails.CurrentAbilityUseAmount = AbilityDetails.MaximumAbilityUseAmount;
}

void AMFG_AbilityFireball::CastAbility(AMFG_Character* AbilityCaster)
{
	Super::CastAbility(AbilityCaster);

	SetAbilityBehavior(AbilityCaster);
}

void AMFG_AbilityFireball::SetAbilityBehavior(AMFG_Character* AbilityCaster)
{
	Super::SetAbilityBehavior(AbilityCaster);

	//CastedFireballs.Empty();

	for (int i = 0; i < AbilityDetails.CurrentAbilityUseAmount; i++)
	{
		AActor* FireballActor = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), FireballProjectileClass, AbilityCaster->GetTransform(), ESpawnActorCollisionHandlingMethod::AlwaysSpawn, AbilityCaster);
		if (IsValid(FireballActor))
		{
			AMFG_FireballProjectile* FireballProjectile = Cast<AMFG_FireballProjectile>(FireballActor);
			if (IsValid(FireballProjectile))
			{
				CastedFireballs.Add(FireballProjectile);
				if (AbilityCaster->GetCharacterType() == EMFG_CharacterType::CharacterType_Player)
				{
					FVector EyeLocation;
					FRotator EyeRotation;

					AbilityCaster->GetActorEyesViewPoint(EyeLocation, EyeRotation);

					FVector ShotDirection = EyeRotation.Vector();
					FVector TraceEnd = EyeLocation + (ShotDirection * 10000);

					FCollisionQueryParams QueryParams;
					QueryParams.AddIgnoredActor(this);
					QueryParams.bTraceComplex = true;

					FVector TraceEndPoint = TraceEnd;

					FHitResult HitResult;
					bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEnd, ECC_GameTraceChannel2, QueryParams);

					if (bHit)
					{
						AMFG_Enemy* EnemyHit = Cast<AMFG_Enemy>(HitResult.GetActor());
						if (IsValid(EnemyHit))
						{
							FireballProjectile->SetTargetCharacter(EnemyHit);
						}
					}
				}
				else
				{
					FireballProjectile->SetTargetCharacter(PlayerCharacterReference);
				}
			}

			UGameplayStatics::FinishSpawningActor(FireballActor, AbilityCaster->GetTransform());
		}
	}

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ShotFireball, this, &AMFG_AbilityFireball::ShotFireball, TimeToShot, false);
}

void AMFG_AbilityFireball::ShotFireball()
{
	TArray<AMFG_FireballProjectile*> DestroyedFireballs;
	for (AMFG_FireballProjectile* FireballProjectile : CastedFireballs)
	{
		if (IsValid(FireballProjectile))
		{
			USphereComponent* FireballCollision = FireballProjectile->GetProjectileCollisionComponent();
			UProjectileMovementComponent* FireballMovementComponent = FireballProjectile->GetProjectileMovementComponent();

			if (IsValid(FireballCollision))
			{
				FireballCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
			}

			if (IsValid(FireballMovementComponent))
			{
				FireballMovementComponent->InitialSpeed = ProjectileInitialSpeed;
				FireballMovementComponent->Activate(true);
			}
		}
		else
		{
			DestroyedFireballs.Add(FireballProjectile);
		}
	}

	for (AMFG_FireballProjectile* FireballProjectile : DestroyedFireballs)
	{
		CastedFireballs.Remove(FireballProjectile);
	}
}
