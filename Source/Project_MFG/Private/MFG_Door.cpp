// Fill out your copyright notice in the Description page of Project Settings.


#include "MFG_Door.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "MFG_Character.h"

// Sets default values
AMFG_Door::AMFG_Door()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	RootComponent = CustomRootComponent;

	DoorFrameComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	DoorFrameComponent->SetupAttachment(CustomRootComponent);

	DoorComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	DoorComponent->SetupAttachment(CustomRootComponent);

	KeyZoneColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("KeyZoneColliderComponent"));
	KeyZoneColliderComponent->SetupAttachment(CustomRootComponent);
	KeyZoneColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	KeyZoneColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	KeyZoneColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	DoorAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("DoorAudioComponent"));

	OpenAngle = -90.0f;
	CloseAngle = 0.0f;
	DoorTag = "KeyA";
	DoorParamName = "Door";
}

// Called when the game starts or when spawned
void AMFG_Door::BeginPlay()
{
	Super::BeginPlay();
	KeyZoneColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &AMFG_Door::CheckKeyFromPlayer);
}

void AMFG_Door::CheckKeyFromPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsOpen)
	{
		return;
	}

	if (!bCanOpenDoor)
	{
		return;
	}

	if (IsValid(OtherActor))
	{
		AMFG_Character* OverlappedCharacter = Cast<AMFG_Character>(OtherActor);
		if (IsValid(OverlappedCharacter) && OverlappedCharacter->GetCharacterType() == EMFG_CharacterType::CharacterType_Player)
		{
			if (OverlappedCharacter->HasKey(DoorTag))
			{
				OpenDoor();
			}
		}
	}
}

// Called every frame
void AMFG_Door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMFG_Door::OpenDoor()
{
// 	FRotator NewRotation = FRotator(0.0f, OpenAngle, 0.0f);
// 	DoorComponent->SetRelativeRotation(NewRotation);
	bIsOpen = true;

	ChangeDoorSound();
	PlayDoorSound(DoorBehaviorSound);

	BP_OpenDoor();
}

void AMFG_Door::CloseDoor()
{
	bIsOpen = false;

	ChangeDoorSound();
	PlayDoorSound(DoorBehaviorSound);

	BP_CloseDoor();
}

void AMFG_Door::ChangeDoorSound()
{
	DoorAudioComponent->SetBoolParameter(DoorParamName, bIsOpen);
}

void AMFG_Door::PlayDoorSound(USoundCue* DoorSound)
{
	if (!IsValid(DoorSound))
	{
		return;
	}

	DoorAudioComponent->SetSound(DoorSound);
	DoorAudioComponent->Play();
}

