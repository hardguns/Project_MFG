// Fill out your copyright notice in the Description page of Project Settings.


#include "MFG_EPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMFG_EPlatform::AMFG_EPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	RootComponent = CustomRootComponent;

	PlatformMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	PlatformMeshComponent->SetupAttachment(CustomRootComponent);

	bIsActive = false;
	bIsGoingUp = false;
	minHeight = 0.0f;
	maxHeight = 1290.0f;
	Speed = 10.0f;

}

// Called when the game starts or when spawned
void AMFG_EPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMFG_EPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsActive)
	{
	   Move();
	}
}

void AMFG_EPlatform::Move()
{
	FVector upDirection = UKismetMathLibrary::GetUpVector(GetActorRotation());
	if (bIsGoingUp)
	{
		upDirection = upDirection;
	}
	else {
		upDirection *= -1;
	}

	FVector currentVelocity = upDirection * Speed;
	FVector newPosition = GetActorLocation() + currentVelocity;
	SetActorLocation(newPosition);

	if ((bIsGoingUp && GetActorLocation().Z >= maxHeight) || (!bIsGoingUp && GetActorLocation().Z <= minHeight))
	{
		bIsGoingUp = !bIsGoingUp;
	}
}

