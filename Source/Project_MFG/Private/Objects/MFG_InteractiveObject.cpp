// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/MFG_InteractiveObject.h"
#include "Components/BoxComponent.h"
#include "MFG_Character.h"

// Sets default values
AMFG_InteractiveObject::AMFG_InteractiveObject()
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

	bSwitchState = false;

}

// Called when the game starts or when spawned
void AMFG_InteractiveObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMFG_InteractiveObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMFG_InteractiveObject::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (IsValid(OtherActor))
	{
		AMFG_Character* OverlappedCharacter = Cast<AMFG_Character>(OtherActor);
		if (IsValid(OverlappedCharacter))
		{
			/*OverlappedCharacter->InteractiveObject = this;*/
			Interact(OverlappedCharacter);
		}
	}
}

void AMFG_InteractiveObject::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (IsValid(OtherActor))
	{
		AMFG_Character* OverlappedCharacter = Cast<AMFG_Character>(OtherActor);
		if (IsValid(OverlappedCharacter))
		{
			//OverlappedCharacter->InteractiveObject = NULL;
			StopInteract(OverlappedCharacter);
		}
	}
}

void AMFG_InteractiveObject::Interact(AMFG_Character* InteractingCharacter)
{
	BP_Interact(InteractingCharacter);
}

void AMFG_InteractiveObject::StopInteract(AMFG_Character* InteractingCharacter)
{
	BP_StopInteract(InteractingCharacter);
}

