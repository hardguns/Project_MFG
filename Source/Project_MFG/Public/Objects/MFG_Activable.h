// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/MFG_InteractiveObject.h"
#include "MFG_Activable.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UPointLightComponent;

/**
 * 
 */
UCLASS()
class PROJECT_MFG_API AMFG_Activable : public AMFG_InteractiveObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* ActivatorMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UPointLightComponent* PointLight;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Light")
		float LightIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activables")
		TArray<AMFG_InteractiveObject*> ActivableObjects;

public:

	AMFG_Activable();

public:

	void UseActivable();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Activables")
	void BP_UseActivator();

protected:

	virtual void Interact(AMFG_Character* OtherActor) override;

	virtual void StopInteract(AMFG_Character* OtherActor) override;

};
