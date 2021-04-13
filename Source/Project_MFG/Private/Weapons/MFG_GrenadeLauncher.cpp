// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/MFG_GrenadeLauncher.h"
#include "MFG_Character.h"
#include "Project_MFG/Project_MFG.h"
#include "Weapons/MFG_Projectile.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"

AMFG_GrenadeLauncher::AMFG_GrenadeLauncher()
{
	MuzzleSocketName = "SCK_Muzzle";

	TraceLenght = 1000.0f;
}

void AMFG_GrenadeLauncher::StartAction()
{
	Super::StartAction();

	if (CurrentOwnerCharacter)
	{
		FVector EyeLocation;
		FRotator EyeRotation;

		CurrentOwnerCharacter->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + (ShotDirection * TraceLenght);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(CurrentOwnerCharacter);
		QueryParams.bTraceComplex = true;

		FVector TraceEndPoint = TraceEnd;

		FHitResult HitResult;
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEnd, COLLISION_ENEMY, QueryParams);

		if (bHit)
		{
			TraceEndPoint = HitResult.ImpactPoint;
		}

		USkeletalMeshComponent* CharacterMeshComponent = CurrentOwnerCharacter->GetMesh();
		if (IsValid(CharacterMeshComponent))
		{
			FVector MuzzleSocketLocation = CharacterMeshComponent->GetSocketLocation(MuzzleSocketName);

			DrawDebugLine(GetWorld(), EyeLocation, TraceEndPoint, FColor::White, false, 1.0f, 0.0f, 1.0f);
			FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleSocketLocation, TraceEndPoint);

			PlaySound(ShotSound);

			AMFG_Projectile* CurrentProjectile = GetWorld()->SpawnActor<AMFG_Projectile>(ProjectileClass, MuzzleSocketLocation, NewRotation);
			CurrentProjectile->SetNewDamageValue(Damage);

			/*AMFG_Character* Character = Cast<AMFG_Character>(CurrentOwnerCharacter);
			if (IsValid(Character))
			{
				if (Character->GetIsUsingUltimate())
				{
					CurrentProjectile->SetNewDamageValue(Damage * Character->GetMultiplier());
				}
			}*/
		}
	}
}

void AMFG_GrenadeLauncher::StopAction()
{
	Super::StopAction();
}
