// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/MFG_Projectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Gameframework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"
#include "MFG_Character.h"

// Sets default values
AMFG_Projectile::AMFG_Projectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollision"));
	RootComponent = ProjectileCollision;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(ProjectileCollision);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 2500.0f;
	ProjectileMovementComponent->MaxSpeed = 2500.0f;
	ProjectileMovementComponent->SetUpdatedComponent(ProjectileCollision);

	InitialLifeSpan = 3.0f;
	DamageRadius = 250.0f;
	Damage = 30.0f;
}

// Called when the game starts or when spawned
void AMFG_Projectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(InitialLifeSpan);
	GetWorldTimerManager().SetTimer(TimerHandle_HandleExplosion, this, &AMFG_Projectile::OnDetonate, 2.0f, false);

}

void AMFG_Projectile::OnDetonate()
{
	FCollisionShape CollisionShape;
	CollisionShape.ShapeType = ECollisionShape::Sphere;
	CollisionShape.SetSphere(DamageRadius);

	TArray<FHitResult> HitResults;
	bool bHit = GetWorld()->SweepMultiByChannel(HitResults, GetActorLocation(), GetActorLocation(), FQuat::FQuat(), ECC_WorldStatic, CollisionShape);

	if (bHit)
	{
		/*for (int i = 0; i < HitResults.Num(); i++)
		{
			AActor* HitActor = HitResults[i].GetActor();*/

			/*if (IsValid(HitActor))
			{*/
		UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), DamageRadius, DamageType, TArray<AActor*>(), this, (AController*)GetOwner(), true, ECC_Visibility);
		/*UGameplayStatics::ApplyPointDamage(HitActor, Damage, ShotDirection, HitResults[i], CurrentOwner->GetInstigatorController(), this, DamageType);*/
			//}
		//}
	}

	if (IsValid(ExplosionEffect))
	{
		UParticleSystemComponent* Explosion = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorTransform());
	}

	//UGameplayStatics::ApplyRadialDamageWithFalloff()

	DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 10.0f, FColor::Red, false, 1.0f, NULL, 1.0f);

	Destroy();
}

// Called every frame
void AMFG_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLifeSpan() <= 0)
	{
		OnDetonate();
	}

}

void AMFG_Projectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (IsValid(OtherActor))
	{
		TArray<AActor*> OverlappedActors;
		GetOverlappingActors(OverlappedActors);
		OnDetonate();
	}
}

