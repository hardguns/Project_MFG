// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "MFG_ANAbilitySound.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MFG_API UMFG_ANAbilitySound : public UAnimNotify
{
	GENERATED_BODY()

	//Gets ability name in AnimMontage
	UPROPERTY(EditAnywhere, Category = "Setup")
	FName AbilityName;

	//Notifies when to reproduce sound
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
