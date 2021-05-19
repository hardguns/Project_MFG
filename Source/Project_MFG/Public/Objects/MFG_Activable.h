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

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* ActivatorMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UPointLightComponent* PointLight;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Light")
		float LightIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Activable Action")
		bool bNeedsKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Activable Action")
		bool bCanBeActivatedOnce; 
		
	UPROPERTY(BlueprintReadOnly, Category = "Activable Action")
		bool bWasActivated;

	//Saves activable tag to active or decativate object (This is only used when activable needs a key)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Activable Action")
		FName ActivableTag;

	//When activable is used, it will active these objects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Activables")
		TArray<AActor*> ActivableObjects;

public:

	AMFG_Activable();

public:

	virtual void CheckActivable(AMFG_Character* OtherActor);

	virtual void UseActivable();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Activables")
	void BP_UseActivator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//When begin overlapped, sets this object to player for interaction
	virtual void Interact(AMFG_Character* OtherActor) override;

	//When end overlapped, unsets this object to player for interaction
	virtual void StopInteract(AMFG_Character* OtherActor) override;

	virtual void PlayInteractionSound(USoundCue* ActivableSound) override;

	void ChangeActivableSound(bool bIsActive);

};
