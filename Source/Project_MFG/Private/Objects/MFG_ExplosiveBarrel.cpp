// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/MFG_ExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/MFG_HealthComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Project_MFG/Project_MFG.h"
#include "GameFramework//Character.h"

// Sets default values
AMFG_ExplosiveBarrel::AMFG_ExplosiveBarrel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRootComponent"));
	RootComponent = CustomRootComponent;

	BarrelMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMeshComponent"));
	BarrelMeshComponent->SetupAttachment(CustomRootComponent);

	BarrelColliderComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BarrelColliderComponent"));
	BarrelColliderComponent->SetupAttachment(CustomRootComponent);
	BarrelColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BarrelColliderComponent->SetCollisionResponseToChannels(ECR_Ignore);
	BarrelColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	HealthComponent = CreateDefaultSubobject<UMFG_HealthComponent>(TEXT("HealthComponent"));

	Damage = 40.0f;
	DamageRadius = 100.0f;
	TimeToExplode = 4.0f;

	ActorsEnum.Add(EObjectTypeQuery::ObjectTypeQuery1);
	ActorsEnum.Add(EObjectTypeQuery::ObjectTypeQuery3);
	ActorsEnum.Add(EObjectTypeQuery::ObjectTypeQuery7);

}

// Called when the game starts or when spawned
void AMFG_ExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &AMFG_ExplosiveBarrel::OnHealthChange);

}

// Called every frame
void AMFG_ExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMFG_ExplosiveBarrel::OnHealthChange(UMFG_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float DamageReceived, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (HealthComponent->GetCurrentHealth() <= 25)
	{
		FName attachName = "Barrel";

		if (IsValid(BurningEffect))
		{
			UGameplayStatics::SpawnEmitterAttached(BurningEffect, CustomRootComponent, attachName);
		}

		GetWorldTimerManager().SetTimer(TimerHandle_HandleExplosion, this, &AMFG_ExplosiveBarrel::Explode, TimeToExplode, false);
	}

	if (HealthComponent->IsDead())
	{
		Explode();
	}
}

void AMFG_ExplosiveBarrel::Explode()
{
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), DamageRadius, ActorsEnum, nullptr, TArray<AActor*>(), ActorsToDamage);

	for (AActor* ActorToDamage : ActorsToDamage)
	{
		if (IsValid(ActorToDamage))
		{
			ACharacter* CharacterToDamage = Cast<ACharacter>(ActorToDamage);
			if (IsValid(CharacterToDamage))
			{
				//UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), DamageRadius, DamageDealType, TArray<AActor*>(), this, GetInstigatorController(), true, ECC_Visibility);
				UGameplayStatics::ApplyDamage(ActorToDamage, Damage, GetInstigatorController(), this, DamageDealType);
				GetWorldTimerManager().ClearTimer(TimerHandle_HandleExplosion);
			}
		}

	}

	if (IsValid(ExplosionEffect))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation(), FVector(3.0f, 3.0f, 3.0f));
		Destroy();
	}
}

