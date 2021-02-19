// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/MFG_Item.h"
#include "MFG_HealthBox.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_MFG_API AMFG_HealthBox : public AMFG_Item
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* HealthBoxMeshComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Box")
		float HealthValue;

public:

	// Sets default values for this actor's properties
	AMFG_HealthBox();

public:

	UFUNCTION(BlueprintCallable, Category = "Getter")
		float GetHealthValue() const { return HealthValue; };

protected:

	virtual void Pickup(AMFG_Character* PickupCharacter) override;
	
};
