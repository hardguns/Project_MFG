// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/MFG_LaserProjectile.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MFG_Character.h"

// Sets default values
AMFG_LaserProjectile::AMFG_LaserProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRootComponent"));
	RootComponent = CustomRootComponent;

	ProjectileCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ProjectileCollision"));
	ProjectileCollision->SetupAttachment(CustomRootComponent);
	ProjectileCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(CustomRootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 4000.0f;
	ProjectileMovementComponent->MaxSpeed = 4000.0f;
	ProjectileMovementComponent->SetUpdatedComponent(CustomRootComponent);

	InitialLifeSpan = 3.0f;
	Damage = 15.0f;

}

// Called when the game starts or when spawned
void AMFG_LaserProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileCollision->OnComponentBeginOverlap.AddDynamic(this, &AMFG_LaserProjectile::OnLaserOverlap);
	ProjectileCollision->OnComponentHit.AddDynamic(this, &AMFG_LaserProjectile::OnLaserHit);
}

void AMFG_LaserProjectile::OnLaserOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, DamageType);

		//if (IsValid(HitEffect))
		//{
			//UParticleSystemComponent* Explosion = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, GetActorTransform());
		//}

		Destroy();
	}
}

void AMFG_LaserProjectile::OnLaserHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}

// Called every frame
void AMFG_LaserProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMFG_LaserProjectile::SetNewDamageValue(float NewDamage)
{
	Damage = NewDamage;
}

