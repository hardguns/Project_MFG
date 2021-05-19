// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/MFG_InteractiveObject.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Core/MFG_GameMode.h"
#include "Sound/SoundCue.h"
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

	InteractiveAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("InteractiveAudioComponent"));
	InteractiveAudioComponent->SetupAttachment(RootComponent);

	bSwitchState = false;
	InteractionParamName = "Interaction";

}

// Called when the game starts or when spawned
void AMFG_InteractiveObject::BeginPlay()
{
	Super::BeginPlay();
	
	GameModeReference = Cast<AMFG_GameMode>(GetWorld()->GetAuthGameMode());
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

//Controls if interactive object is active
void AMFG_InteractiveObject::SetObjectActiveState(bool NewState)
{
	bSwitchState = NewState;
}

void AMFG_InteractiveObject::PlayInteractionSound(USoundCue* ObjectSound)
{
	if (!IsValid(ObjectSound))
	{
		return;
	}

	InteractiveAudioComponent->SetSound(ObjectSound);
	InteractiveAudioComponent->Play();
}

void AMFG_InteractiveObject::HitObject()
{
	
}

//Called in child to do different actions
void AMFG_InteractiveObject::Interact(AMFG_Character* InteractingCharacter)
{
	BP_Interact(InteractingCharacter);
}

//Called in child to do different actions
void AMFG_InteractiveObject::StopInteract(AMFG_Character* InteractingCharacter)
{
	BP_StopInteract(InteractingCharacter);
}

