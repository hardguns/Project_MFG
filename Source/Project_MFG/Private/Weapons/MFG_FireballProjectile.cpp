// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/MFG_FireballProjectile.h"
#include "Weapons/MFG_Projectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/MFG_HealthComponent.h"
#include "MFG_Character.h"
#include "Project_MFG/Project_MFG.h"

AMFG_FireballProjectile::AMFG_FireballProjectile()
{
	FireballEffectComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireballEffectComponent"));
	FireballEffectComponent->SetupAttachment(RootComponent);

	FireballHealthComponent = CreateDefaultSubobject<UMFG_HealthComponent>(TEXT("FireballHealthComponent"));

	ProjectileMovementComponent->bAutoActivate = false;
	HomingStrength = 15000.0f;
	bCanMove = true;
}

void AMFG_FireballProjectile::BeginPlay()
{
	Super::BeginPlay();

	FireballHealthComponent->OnHealthChangeDelegate.AddDynamic(this, &AMFG_FireballProjectile::OnHealthChange);

	if (SpawnEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnEffect, GetActorLocation(), GetActorRotation());
	}
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle_ReachDestination, this, &AMFG_FireballProjectile::ReachDestination, 0.1f, true);
}

void AMFG_FireballProjectile::Tick(float DeltaTime)
{
	if (bCanMove)
	{
		ReachDestination(DeltaTime);
	}
}

void AMFG_FireballProjectile::ReachDestination(float DeltaTime)
{
	if (IsValid(TargetCharacter))
	{
		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetCharacter->GetActorLocation());
		UKismetMathLibrary::RInterpTo(GetActorRotation(), NewRotation, DeltaTime, HomingStrength);
		SetActorRotation(NewRotation);
		FVector NewPosition = (GetActorForwardVector() * ProjectileMovementComponent->InitialSpeed) + GetActorLocation();
		SetActorRelativeLocation(NewPosition);
	}
	else
	{
		FVector NewPosition = (GetActorForwardVector() * ProjectileMovementComponent->InitialSpeed) + GetActorLocation();
		SetActorRelativeLocation(NewPosition);
	}
}

void AMFG_FireballProjectile::OnHealthChange(UMFG_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float DamageDealed, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (IsValid(DamagedActor) && CurrentHealthComponent->IsDead())
	{
		Destroy();
	}
}
