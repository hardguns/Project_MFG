// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/MFG_ProximityMine.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/MFG_HealthComponent.h"
#include "MFG_Character.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PointLightComponent.h"
#include "GameFramework/Character.h"
#include "Sound/SoundCue.h"

// Sets default values
AMFG_ProximityMine::AMFG_ProximityMine()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterDistance = 0.0f;
	DetonationDistance = 200.0f;
	DamageRadius = 80.0f;
	LightIntensity = 2000.0f;

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRootComponent"));
	RootComponent = CustomRootComponent;

	MineMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MineMeshComponent"));
	MineMeshComponent->SetupAttachment(CustomRootComponent);

	MineLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("MineLightComponent"));
	MineLight->SetupAttachment(CustomRootComponent);
	MineLight->Intensity = LightIntensity;
	MineLight->SetLightColor(FColor::Orange);

	SphereColliderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereColliderComponent"));
	SphereColliderComponent->SetupAttachment(CustomRootComponent);
	SphereColliderComponent->SetSphereRadius(DamageRadius);
	SphereColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	HealthComponent = CreateDefaultSubobject<UMFG_HealthComponent>(TEXT("HealthComponent"));

	Damage = 30.0f;
	XPValue = 20.0f;

	CurrentActor = NULL;

	ActorsEnum.Add(EObjectTypeQuery::ObjectTypeQuery1);
	ActorsEnum.Add(EObjectTypeQuery::ObjectTypeQuery3);
	ActorsEnum.Add(EObjectTypeQuery::ObjectTypeQuery7);

}

// Called when the game starts or when spawned
void AMFG_ProximityMine::BeginPlay()
{
	Super::BeginPlay();

	SphereColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &AMFG_ProximityMine::EnterActivationZone);
	SphereColliderComponent->OnComponentEndOverlap.AddDynamic(this, &AMFG_ProximityMine::ExitActivationZone);

	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &AMFG_ProximityMine::OnHealthChange);
}

// Called every frame
void AMFG_ProximityMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ValidateDistance();
}

void AMFG_ProximityMine::Explode()
{
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), DamageRadius, ActorsEnum, nullptr, TArray<AActor*>(), ActorsToDamage);

	for (AActor* ActorToDamage : ActorsToDamage)
	{
		if (IsValid(ActorToDamage))
		{
			ACharacter* CharacterToDamage = Cast<ACharacter>(ActorToDamage);
			if (IsValid(CharacterToDamage))
			{
				UGameplayStatics::ApplyDamage(ActorToDamage, Damage, GetInstigatorController(), this, DamageDealType);
				//UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), DamageRadius, DamageDealType, TArray<AActor*>(), this, GetInstigatorController(), true, ECC_Visibility);
			}
		}
	}

	if (IsValid(ExplosionEffect))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation(), FVector(3.0f, 3.0f, 3.0f));
	}

	PlayExplosionSound();

	Destroy();
}

void AMFG_ProximityMine::ValidateDistance()
{
	if (IsValid(CurrentActor))
	{
		CharacterDistance = GetDistanceTo(CurrentActor);
		if (CharacterDistance <= DetonationDistance)
		{
			Explode();
		}

		if (bDebug)
		{
			UE_LOG(LogTemp, Log, TEXT("Distance from actor: %s"), *FString::SanitizeFloat(CharacterDistance));
		}
	}
}

void AMFG_ProximityMine::EnterActivationZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		AMFG_Character* Character = Cast<AMFG_Character>(OtherActor);
		if (IsValid(Character))
		{
			CurrentActor = Character;
			CharacterDistance = GetDistanceTo(CurrentActor);

			MineLight->SetLightColor(FColor::Red);

		}
	}
}

void AMFG_ProximityMine::ExitActivationZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor))
	{
		AMFG_Character* Character = Cast<AMFG_Character>(OtherActor);
		if (IsValid(Character))
		{
			MineLight->SetLightColor(FColor::Orange);
			CurrentActor = NULL;
		}
	}
}

void AMFG_ProximityMine::OnHealthChange(UMFG_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float DamageReceived, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (HealthComponent->IsDead())
	{
		if (IsValid(DamageCauser))
		{
			AActor* WeaponOwner = DamageCauser->GetOwner();
			if (IsValid(WeaponOwner))
			{
				AMFG_Character* Character = Cast<AMFG_Character>(WeaponOwner);
				if (IsValid(Character))
				{
					Character->GainUltimateXP(XPValue);
				}
			}
		}
		Explode();
	}
}

void AMFG_ProximityMine::PlayExplosionSound()
{
	if (!IsValid(ExplosionSound))
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
}


