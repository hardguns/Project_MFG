// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Controller/MFG_AIController.h"
#include "AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "AIModule/Classes/Perception/AIPerceptionComponent.h"
#include "Enemy/MFG_Enemy.h"


AMFG_AIController::AMFG_AIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	
	LoopPathParameterName = "bLoopPath";
	DirectionIndexParameter = "DirectionIndex";
	WaitingTimeParameterName = "WaitingOnPathPoint";
	CanSeePlayerParameterName = "bCanSeePlayer";
	InvestigatingParameterName = "bIsInvestigating";
	TargetLocationParameterName = "TargetLocation";
}

void AMFG_AIController::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(EnemyBehaviorTree))
	{
		RunBehaviorTree(EnemyBehaviorTree);
	}

	MyEnemy = Cast<AMFG_Enemy>(K2_GetPawn());
	if (IsValid(MyEnemy))
	{
		MyBlackboard = UAIBlueprintHelperLibrary::GetBlackboard(this);

		if (IsValid(MyBlackboard))
		{
			MyBlackboard->SetValueAsBool(LoopPathParameterName, MyEnemy->GetLoopPath());
			MyBlackboard->SetValueAsInt(DirectionIndexParameter, MyEnemy->GetDirectionIndex());
			MyBlackboard->SetValueAsFloat(WaitingTimeParameterName, MyEnemy->GetWaitingTime());
		}
	}

	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AMFG_AIController::UpdateSenses);
}

void AMFG_AIController::UpdateSenses(const TArray<AActor*>& UpdatedActors)
{
	if (!IsValid(MyBlackboard))
	{
		return;
	}

	for (AActor* Actor : UpdatedActors)
	{
		FActorPerceptionBlueprintInfo PerceptionInfo;
		AIPerceptionComponent->GetActorsPerception(Actor, PerceptionInfo);

		AMFG_Character* SensedCharacter = Cast<AMFG_Character>(Actor);
		if (IsValid(SensedCharacter) && SensedCharacter->GetCharacterType() == EMFG_CharacterType::CharacterType_Player)
		{
			for (int i = 0; i < PerceptionInfo.LastSensedStimuli.Num(); i++)
			{
				switch (i)
				{
				case 0:
					MyBlackboard->SetValueAsBool(CanSeePlayerParameterName, PerceptionInfo.LastSensedStimuli[i].WasSuccessfullySensed());

					if (IsValid(MyEnemy))
					{
						MyEnemy->SetAlert(PerceptionInfo.LastSensedStimuli[i].WasSuccessfullySensed());
					}

					break;
				case 1:
					MyBlackboard->SetValueAsBool(InvestigatingParameterName, bReceivingDamage);
					if (bReceivingDamage)
					{
						MyBlackboard->SetValueAsVector(TargetLocationParameterName, PerceptionInfo.LastSensedStimuli[i].StimulusLocation);
					}
					break;
				default:
					break;
				}
			}
		}
	}
}

void AMFG_AIController::DeactivateAIPerception()
{
	AIPerceptionComponent->Deactivate();
}
