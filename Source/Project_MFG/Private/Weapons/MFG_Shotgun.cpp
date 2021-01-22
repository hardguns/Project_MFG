// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/MFG_Shotgun.h"
#include "Project_MFG/Project_MFG.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/Character.h"
#include "Particles/ParticleSystemComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "MFG_Character.h"
#include "GameFramework/CharacterMovementComponent.h"

AMFG_Shotgun::AMFG_Shotgun()
{
	Damage = 60;
	TraceLenght = 850.0f;
	MuzzleSocketName = "SCK_Muzzle";
	WeaponRecoilRowName = "Shotgun";
	PelletsNumber = 5;
	ShotForce = 1500.0f;
	CanShoot = true;
}

void AMFG_Shotgun::BeginPlay()
{
	Super::BeginPlay();

	GetRecoilInfo(WeaponRecoilRowName);
}

void AMFG_Shotgun::StartAction()
{
	Super::StartAction();

	if (CanShoot)
	{
		ActionShot();
	}
}

void AMFG_Shotgun::StopAction()
{
	Super::StopAction();
}

void AMFG_Shotgun::ActionShot()
{
	AActor* CurrentOwner = GetOwner();

	if (IsValid(CurrentOwner))
	{
		FVector EyeLocation;
		FRotator EyeRotation;

		AMFG_Character* CurrentCharacter = Cast<AMFG_Character>(CurrentOwner);

		if (IsValid(CurrentCharacter))
		{
			WeaponPitchRecoil = CurrentCharacter->GetCharacterType() == EMFG_CharacterType::CharacterType_Enemy ? FMath::RandRange(EnemyBehaviorRow->MinPitchRecoil, EnemyBehaviorRow->MaxPitchRecoil) : FMath::RandRange(CharacterBehaviorRow->MinPitchRecoil, CharacterBehaviorRow->MaxPitchRecoil);
			WeaponYawRecoil = CurrentCharacter->GetCharacterType() == EMFG_CharacterType::CharacterType_Enemy ? FMath::RandRange(EnemyBehaviorRow->MinYawRecoil, EnemyBehaviorRow->MaxYawRecoil) : FMath::RandRange(CharacterBehaviorRow->MinYawRecoil, CharacterBehaviorRow->MaxYawRecoil);
		}

		CameraPitchRecoilShake = FMath::RandRange(CharacterBehaviorRow->MinCameraPitchShake, CharacterBehaviorRow->MaxCameraPitchShake);

		CurrentOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
		EyeRotation.Pitch = EyeRotation.Pitch + WeaponPitchRecoil;
		EyeRotation.Yaw = EyeRotation.Yaw + WeaponYawRecoil;

		FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + (ShotDirection * TraceLenght);

		FVector TraceEndPoint = TraceEnd;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(CurrentOwner);
		QueryParams.bTraceComplex = true;
		int SuccessfulHits = 0;

		for (int i = 0; i < PelletsNumber; i++)
		{
			float RandPosX = FMath::RandRange(-15.0f, 15.0f);
			float RandPosY = FMath::RandRange(-15.0f, 15.0f);
			float RandPosZ = FMath::RandRange(-50.0f, 50.0f);

			FVector RandVector = FVector(RandPosX, RandPosY, RandPosZ);
			FVector NewEndTrace = TraceEndPoint + RandVector;

			FHitResult HitResult;
			if (bDrawLineTrace)
			{
				DrawDebugLine(GetWorld(), EyeLocation, NewEndTrace, FColor::Orange, false, 2.0f, 0, 1.0f);
			}

			bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, NewEndTrace, COLLISION_WEAPON, QueryParams);
			if (bHit)
			{
				AActor* HitActor = HitResult.GetActor();

				if (IsValid(HitActor))
				{
					UGameplayStatics::ApplyPointDamage(HitActor, CalculateDamage(1), ShotDirection, HitResult, CurrentOwner->GetInstigatorController(), this, DamageType);
				}

				TraceEndPoint = HitResult.ImpactPoint;

				if (IsValid(ImpactEffect))
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, TraceEndPoint, HitResult.ImpactNormal.Rotation());
				}
			}
		}

		if (IsValid(CurrentCharacter))
		{
			CurrentCharacter->AddControllerPitchInput(CameraPitchRecoilShake);
		}

		/*AMFG_Character* MainCharacter = Cast<AMFG_Character>(CurrentOwner);
		if (IsValid(MainCharacter))
		{
			FVector MovementDirection = -MainCharacter->GetRootComponent()->GetForwardVector();
			if (!MainCharacter->GetMovementComponent()->IsFalling())
			{
				MainCharacter->LaunchCharacter(MovementDirection * ShotForce, true, true);
			}
		}*/

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

float AMFG_Shotgun::CalculateDamage(int numberOfHits)
{
	float DamageDeal = (Damage * numberOfHits) / PelletsNumber;

	return DamageDeal;
}
