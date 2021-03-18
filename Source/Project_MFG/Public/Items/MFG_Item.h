// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MFG_Item.generated.h"

class USphereComponent;
class AMFG_Character;
class AMFG_GameMode;

UCLASS()
class PROJECT_MFG_API AMFG_Item : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USphereComponent* MainColliderComponent;

protected:

	UPROPERTY(BlueprintReadOnly)
	AMFG_GameMode* GameModeReference;

public:
	// Sets default values for this actor's properties
	AMFG_Item();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Pickup(AMFG_Character* PickupCharacter);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Item")
	void BP_Pickup(AMFG_Character* PickupCharacter);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
