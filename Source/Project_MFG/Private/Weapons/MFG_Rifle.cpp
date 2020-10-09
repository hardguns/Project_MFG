// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/MFG_Rifle.h"
#include "Project_MFG/Project_MFG.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/EngineTypes.h"

AMFG_Rifle::AMFG_Rifle()
{
	TraceLenght = 10000.0f;
	bIsAutomatic = false;
	MuzzleSocketName = "SCK_Muzzle";
	TimeBetweenShots = 0.15f;
}
	

void AMFG_Rifle::StartAction()
{
	Super::StartAction();

	ActionShot();

	if (bIsAutomatic)
	{
		GetWorldTimerManager().SetTimer(TimerHandle_HandleAutomaticShot, this, &AMFG_Rifle::ActionShot, TimeBetweenShots, true);
	}
}

void AMFG_Rifle::StopAction()
{
	Super::StopAction();

	if (bIsAutomatic)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_HandleAutomaticShot);
	}
	
}

void AMFG_Rifle::ActionShot()
{
	AActor* CurrentOwner = GetOwner();

	if (IsValid(CurrentOwner))
	{
		FVector EyeLocation;
		FRotator EyeRotation;

		CurrentOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + (ShotDirection * TraceLenght);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(CurrentOwner);
		QueryParams.bTraceComplex = true;

		FVector TraceEndPoint = TraceEnd;

		FHitResult HitResult;
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams);

		if (bHit)
		{
			//Make damage
			AActor* HitActor = HitResult.GetActor();

			if (IsValid(HitActor))
			{
				UGameplayStatics::ApplyPointDamage(HitActor, Damage, ShotDirection, HitResult, CurrentOwner->GetInstigatorController(), this, DamageType);
			}

			TraceEndPoint = HitResult.ImpactPoint;

			if (IsValid(ImpactEffect))
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, TraceEndPoint, HitResult.ImpactNormal.Rotation());
			}

		}

		if (bDrawLineTrace)
		{
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.0f, 0.0f, 1.0f);
		}

		if (IsValid(MuzzleEffect))
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, CurrentOwnerCharacter->GetMesh(), MuzzleSocketName);
		}

		if (IsValid(TraceEffect))
		{
			USkeletalMeshComponent* CharacterMeshComponent = CurrentOwnerCharacter->GetMesh();
			if (IsValid(CharacterMeshComponent))
			{
				FVector MuzzleSocketLocation = CharacterMeshComponent->GetSocketLocation(MuzzleSocketName);
				UParticleSystemComponent* TraceComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TraceEffect, MuzzleSocketLocation);

				if (IsValid(TraceComponent))
				{
					TraceComponent->SetVectorParameter(TraceParamName, TraceEndPoint);
				}
			}
		}
	}
}
