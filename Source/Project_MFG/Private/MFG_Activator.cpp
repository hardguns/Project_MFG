// Fill out your copyright notice in the Description page of Project Settings.


#include "MFG_Activator.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PointLightComponent.h"
#include "MFG_Character.h"
#include "Items/MFG_LaunchPad.h"

// Sets default values
AMFG_Activator::AMFG_Activator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRootComponent"));
	RootComponent = CustomRootComponent;

	ActivatorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ActivatorMeshComponent"));
	ActivatorMeshComponent->SetupAttachment(CustomRootComponent);

	ActiveZoneColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ActiveZoneColliderComponent"));
	ActiveZoneColliderComponent->SetupAttachment(CustomRootComponent);
	ActiveZoneColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ActiveZoneColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	ActiveZoneColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	LightIntensity = 2000.0f;
	bSwitchState = false;

	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLightComponent"));
	PointLight->Intensity = LightIntensity;
	//PointLight->SetLightColor(FLinearColor(222,0,0,0), true);
	PointLight->SetupAttachment(CustomRootComponent);

	ActivatorTag = "LaunchPad1";

}

// Called when the game starts or when spawned
void AMFG_Activator::BeginPlay()
{
	Super::BeginPlay();
	ActiveZoneColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &AMFG_Activator::CheckActivatorUse);
	ActiveZoneColliderComponent->OnComponentEndOverlap.AddDynamic(this, &AMFG_Activator::UnCheckActivatorUse);

}

void AMFG_Activator::CheckActivatorUse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		AMFG_Character* OverlappedCharacter = Cast<AMFG_Character>(OtherActor);
		if (IsValid(OverlappedCharacter))
		{
			OverlappedCharacter->InteractiveObject = this;
		}
	}
}

void AMFG_Activator::UnCheckActivatorUse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor))
	{
		AMFG_Character* OverlappedCharacter = Cast<AMFG_Character>(OtherActor);
		if (IsValid(OverlappedCharacter))
		{
			OverlappedCharacter->InteractiveObject = NULL;
		}
	}
}

// Called every frame
void AMFG_Activator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMFG_Activator::UseActivator()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMFG_Item::StaticClass(), FoundActors);
	for (int i = 0; i < FoundActors.Num(); i++)
	{
		//if (ActivatorTag.ToString().Find("LaunchPad"))
		//{
			AMFG_LaunchPad* LaunchPadObj = Cast<AMFG_LaunchPad>(FoundActors[i]);
			if (IsValid(LaunchPadObj))
			{
				if (LaunchPadObj->LaunchPadTag == ActivatorTag)
				{
					bool activeState = LaunchPadObj->bIsActive;
					LaunchPadObj->bIsActive = !activeState;
					bSwitchState = !activeState;
					BP_UseActivator();
				}
			}
		//}
	}
}

