// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MFG_Character.h"
#include "MFG_Enemy.generated.h"

class AMFG_PathActor;

/**
 * 
 */
UCLASS()
class PROJECT_MFG_API AMFG_Enemy : public AMFG_Character
{
	GENERATED_BODY()

public:

	AMFG_Enemy();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Navigation Path")
		bool bLoopPath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Navigation Path")
		int DirectionIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Navigation Path")
		float WaitingTimeOnPathPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Navigation Path")
		AMFG_PathActor* MyPath;
	
};
