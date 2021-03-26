// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/MFG_AbilityLaser.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapons/MFG_LaserProjectile.h"
#include "MFG_Character.h"
#include "Project_MFG/Project_MFG.h"

AMFG_AbilityLaser::AMFG_AbilityLaser()
{
	LaserTraceLenght = 8000.0f;
	LaserSocketName = "Muzzle_05";
}

void AMFG_AbilityLaser::BeginPlay()
{
	Super::BeginPlay();

	AbilityDetails.CurrentAbilityUseAmount = AbilityDetails.MaximumAbilityUseAmount;
}

void AMFG_AbilityLaser::CastAbility()
{
	Super::CastAbility();

	if (AbilityDetails.CurrentAbilityUseAmount > 0)
	{
		if (IsValid(PlayerCharacterReference))
		{
			//PlayerCharacterReference->SetAbilityState(true);

			UAnimInstance* PlayerAnimInstance = PlayerCharacterReference->GetPlayerAnimInstance();
			if (IsValid(PlayerAnimInstance) && IsValid(LaserShotMontage))
			{
				PlayerAnimInstance->Montage_Play(LaserShotMontage);
			}

			AbilityDetails.CurrentAbilityUseAmount--;

			PlayerCharacterReference->OnAbilityChangeDelegate.Broadcast(AbilityDetails.CurrentAbilityUseAmount, AbilityDetails.AbilityIndex);

			GetWorld()->GetTimerManager().SetTimer(TimerHandle_ReloadAbility, this, &AMFG_AbilityLaser::ReloadAbility, AbilityDetails.AbilityCooldown, true);
		}
	}
}

void AMFG_AbilityLaser::ReloadAbility()
{
	Super::ReloadAbility();

	if (AbilityDetails.CurrentAbilityUseAmount < AbilityDetails.MaximumAbilityUseAmount)
	{
		AbilityDetails.CurrentAbilityUseAmount++;
		if (IsValid(PlayerCharacterReference))
		{
			PlayerCharacterReference->OnAbilityChangeDelegate.Broadcast(AbilityDetails.CurrentAbilityUseAmount, AbilityDetails.AbilityIndex);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ReloadAbility);
	}
}

void AMFG_AbilityLaser::SetAbilityBehavior()
{
	Super::SetAbilityBehavior();

	if (IsValid(PlayerCharacterReference))
	{
		AActor* CurrentCharacterActor = PlayerCharacterReference->GetOwner();

		if (IsValid(CurrentCharacterActor))
		{
			FVector EyeLocation;
			FRotator EyeRotation;

			CurrentCharacterActor->GetActorEyesViewPoint(EyeLocation, EyeRotation);

			FVector ShotDirection = EyeRotation.Vector();
			FVector TraceEnd = EyeLocation + (ShotDirection * LaserTraceLenght);

			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this);
			QueryParams.bTraceComplex = true;

			FVector TraceEndPoint = TraceEnd;

			FHitResult HitResult;
			bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams);

			if (bHit)
			{
				TraceEndPoint = HitResult.ImpactPoint;

				USkeletalMeshComponent* PlayerMesh = PlayerCharacterReference->GetMesh();
				FVector LaserSocketLocation = PlayerMesh->GetSocketLocation(LaserSocketName);
				FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(LaserSocketLocation, TraceEndPoint);

				AMFG_LaserProjectile* CurrentProjectile = GetWorld()->SpawnActor<AMFG_LaserProjectile>(LaserProjectileClass, LaserSocketLocation, NewRotation);
				if (PlayerCharacterReference->GetIsUsingUltimate())
				{
					CurrentProjectile->SetNewDamageValue(CurrentProjectile->GetLaserProjectileDamage() * PlayerCharacterReference->GetUltimateWeaponDamageMultiplier());
				}
			}
		}
	}
}
