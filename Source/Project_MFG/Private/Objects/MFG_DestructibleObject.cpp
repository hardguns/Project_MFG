// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/MFG_DestructibleObject.h"
#include "Components/BoxComponent.h"
#include "Components/MFG_HealthComponent.h"

// Sets default values
AMFG_DestructibleObject::AMFG_DestructibleObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("MainColliderComponent"));
	RootComponent = MainColliderComponent;
	MainColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MainColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MainColliderComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	MainColliderComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	MainColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	HealthComponent = CreateDefaultSubobject<UMFG_HealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void AMFG_DestructibleObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMFG_DestructibleObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

