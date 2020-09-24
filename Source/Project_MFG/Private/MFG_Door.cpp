// Fill out your copyright notice in the Description page of Project Settings.


#include "MFG_Door.h"
#include "Components/StaticMeshComponent.h"

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

	OpenAngle = -90.0f;
}

// Called when the game starts or when spawned
void AMFG_Door::BeginPlay()
{
	Super::BeginPlay();
	OpenDoor();
}

// Called every frame
void AMFG_Door::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMFG_Door::OpenDoor()
{
	FRotator NewRotation = FRotator(0.0f, OpenAngle, 0.0f);
	DoorComponent->SetRelativeRotation(NewRotation);
}

