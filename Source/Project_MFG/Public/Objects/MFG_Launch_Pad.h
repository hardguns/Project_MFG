// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/MFG_InteractiveObject.h"
#include "MFG_Launch_Pad.generated.h"

class UStaticMeshComponent;
class UParticleSystemComponent;

/**
 * 
 */
UCLASS()
class PROJECT_MFG_API AMFG_Launch_Pad : public AMFG_InteractiveObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* LaunchPadMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UParticleSystemComponent* LaunchPadParticleComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaunchPad")
		float LaunchForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaunchPad")
		FVector InitialLaunchDirection;

public:

	AMFG_Launch_Pad();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void Interact(AMFG_Character* CharacterToLaunch) override;

};
