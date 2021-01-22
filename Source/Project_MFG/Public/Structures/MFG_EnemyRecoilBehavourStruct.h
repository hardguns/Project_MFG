// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "MFG_EnemyRecoilBehavourStruct.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MFG_API UMFG_EnemyRecoilBehavourStruct : public UUserDefinedStruct
{
	GENERATED_BODY()

public:

	UMFG_EnemyRecoilBehavourStruct();

public:

	UPROPERTY(EditAnywhere)
		float MinPitchRecoil;

	UPROPERTY(EditAnywhere)
		float MaxPitchRecoil;

	UPROPERTY(EditAnywhere)
		float MinYawRecoil;

	UPROPERTY(EditAnywhere)
		float MaxYawRecoil;

	UPROPERTY(EditAnywhere)
		float MinCameraPitchShake;

	UPROPERTY(EditAnywhere)
		float MaxCameraPitchShake;

	UPROPERTY(EditAnywhere)
		float MinCameraYawShake;

	UPROPERTY(EditAnywhere)
		float MaxCameraYawShake;

	UPROPERTY(EditAnywhere)
		float testVar;
	
};
