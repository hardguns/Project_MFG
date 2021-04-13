// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/MFG_Item.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Core/MFG_GameMode.h"
#include "Sound/SoundCue.h"
#include "MFG_Character.h"

// Sets default values
AMFG_Item::AMFG_Item()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainColliderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("MainColliderComponent"));
	RootComponent = MainColliderComponent;
	MainColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MainColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MainColliderComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	MainColliderComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	MainColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

}

// Called when the game starts or when spawned
void AMFG_Item::BeginPlay()
{
	Super::BeginPlay();
	
	GameModeReference = Cast<AMFG_GameMode>(GetWorld()->GetAuthGameMode());
}

// Called every frame
void AMFG_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Use it when the object's root component is a collider ***********************
void AMFG_Item::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (IsValid(OtherActor))
	{
		AMFG_Character* OverlappedCharacter = Cast<AMFG_Character>(OtherActor);
		if (IsValid(OverlappedCharacter) && OverlappedCharacter->GetCharacterType() == EMFG_CharacterType::CharacterType_Player)
		{
			Pickup(OverlappedCharacter);
		}
	}
}

void AMFG_Item::Pickup(AMFG_Character* PickupCharacter)
{
	BP_Pickup(PickupCharacter);
}

void AMFG_Item::PlayPickupSound()
{
	if (!IsValid(PickupSound))
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
}

