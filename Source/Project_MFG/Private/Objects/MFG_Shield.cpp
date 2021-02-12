// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/MFG_Shield.h"
#include "Components/StaticMeshComponent.h"
#include "Components/MFG_HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "MFG_Character.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMFG_Shield::AMFG_Shield()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShieldMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMeshComponent"));
	RootComponent = ShieldMeshComponent;

	HealthComponent = CreateDefaultSubobject<UMFG_HealthComponent>(TEXT("HealthComponent"));

	HitCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HitBoxComponent"));
	HitCapsuleComponent->SetupAttachment(RootComponent);
	HitCapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

	HitCounter = 0;

	ShieldColors.Add(UKismetMathLibrary::MakeColor(15,0,120,0));
	ShieldColors.Add(UKismetMathLibrary::MakeColor(120,0,75,0));
	ShieldColors.Add(UKismetMathLibrary::MakeColor(120,0,10,0));
}

// Called when the game starts or when spawned
void AMFG_Shield::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &AMFG_Shield::TakingDamage);

	ShieldMaterial = ShieldMeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, ShieldMeshComponent->GetMaterial(0));

}

void AMFG_Shield::TakingDamage(UMFG_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	HitCounter++;

	if (IsValid(ShieldMaterial))
	{
		//FLinearColor CurrentColor;
		//ShieldMaterial->GetVectorParameterValue(FName("Color"), CurrentColor);

		ShieldMaterial->SetVectorParameterValue(FName("Color"), ShieldColors[HitCounter - 1]);
	}

	if (CurrentHealthComponent->IsDead())
	{
		DestroyShield();
	}
}

// Called every frame
void AMFG_Shield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMFG_Shield::SetShieldOwner(AMFG_Character* NewOwner)
{
	if (IsValid(NewOwner))
	{
		SetOwner(NewOwner);
		NewOwner->SetShield(this);
		ShieldOwner = NewOwner;
	}
}

void AMFG_Shield::DestroyShield()
{
	AMFG_Character* CharacterOwner = Cast<AMFG_Character>(GetOwner());
	if (IsValid(CharacterOwner))
	{
		CharacterOwner->SetShield(nullptr);
	}

	HitCounter = 0;
	Destroy();
}

