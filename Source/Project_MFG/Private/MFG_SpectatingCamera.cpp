// Fill out your copyright notice in the Description page of Project Settings.


#include "MFG_SpectatingCamera.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AMFG_SpectatingCamera::AMFG_SpectatingCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpectatingCameraComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpectatingCameraComponent"));
	RootComponent = SpectatingCameraComponent;



}

