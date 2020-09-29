// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/MFG_Item.h"
#include "MFG_LaunchPad.generated.h"

class UStaticMeshComponent;
class UParticleSystemComponent;

/**
 * 
 */
UCLASS()
class PROJECT_MFG_API AMFG_LaunchPad : public AMFG_Item
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

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LaunchPad")
		bool bIsActive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LaunchPad")
		FName LaunchPadTag;

public:

	AMFG_LaunchPad();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void Pickup(AMFG_Character* CharacterToLaunch) override;
	
};
