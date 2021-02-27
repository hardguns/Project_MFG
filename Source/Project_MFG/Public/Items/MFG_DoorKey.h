// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/MFG_Item.h"
#include "MFG_DoorKey.generated.h"

class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class PROJECT_MFG_API AMFG_DoorKey : public AMFG_Item
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* KeyMeshComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
		float XPValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Key")
		FName KeyTag;

public:

	// Sets default values for this actor's properties
	AMFG_DoorKey();

public:
	
	UFUNCTION(BlueprintCallable, Category = "Getter")
	FName GetKeyTag() const { return KeyTag; };

	UFUNCTION(BlueprintCallable, Category = "Setter")
	void SetKeyTag(FName NewTag) { KeyTag = NewTag; };

protected:

	virtual void Pickup(AMFG_Character* PickupCharacter) override;
	
};
