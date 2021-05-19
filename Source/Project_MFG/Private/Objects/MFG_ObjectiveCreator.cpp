// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/MFG_ObjectiveCreator.h"
#include "UI/HUD/MFG_HUDObjective.h"
#include "UI/HUD/MFG_HUDObjectivesContainer.h"
#include "UI/HUD/MFG_HUD.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "MFG_Character.h"

// Sets default values
AMFG_ObjectiveCreator::AMFG_ObjectiveCreator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ObjectiveZoneComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ObjectiveZoneComponent"));
	ObjectiveZoneComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ObjectiveZoneComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	ObjectiveZoneComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = ObjectiveZoneComponent;
}

// Called when the game starts or when spawned
void AMFG_ObjectiveCreator::BeginPlay()
{
	Super::BeginPlay();

	ObjectiveZoneComponent->OnComponentBeginOverlap.AddDynamic(this, &AMFG_ObjectiveCreator::CreateNewObjective);
}

void AMFG_ObjectiveCreator::CreateNewObjective(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (IsValid(PlayerCharacter))
	{
		AMFG_Character* MainCharacter = Cast<AMFG_Character>(PlayerCharacter);
		if (IsValid(MainCharacter) && MainCharacter->GetCharacterType() == EMFG_CharacterType::CharacterType_Player)
		{
			APlayerController* PlayerController = Cast<APlayerController>(MainCharacter->GetController());
			UUserWidget* NewUI = CreateWidget<UMFG_HUDObjective>(PlayerController, UMFG_HUDObjective::StaticClass(), NAME_None);
			if (IsValid(NewUI))
			{
				UMFG_HUDObjective* NewObjective = Cast<UMFG_HUDObjective>(NewUI);
				if (IsValid(NewObjective))
				{
					NewObjective->ObjectiveName = ObjectiveName;
					NewObjective->ObjectiveIdentifier = ObjectiveIdentifier;

					NewObjectives.Add(NewObjective);
				}
			}

			UMFG_HUD* MainHUD = MainCharacter->GetCharacterMainHUD();
			if (IsValid(MainHUD))
			{
				UMFG_HUDObjectivesContainer* ObjectivesContainer = MainHUD->GetObjectivesContainer();
				if (IsValid(ObjectivesContainer))
				{
					if (NewObjectives.Num() > 0)
					{
						ObjectivesContainer->CreateNewObjectives(NewObjectives);

						Destroy();
					}
				}
			}
		}
	}
}

