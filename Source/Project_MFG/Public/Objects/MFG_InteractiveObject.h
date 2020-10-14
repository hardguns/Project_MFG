// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MFG_InteractiveObject.generated.h"

class UBoxComponent;

UCLASS()
class PROJECT_MFG_API AMFG_InteractiveObject : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UBoxComponent* MainColliderComponent;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interactive Object")
		bool bSwitchState;
	
public:	
	// Sets default values for this actor's properties
	AMFG_InteractiveObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Interact(AMFG_Character* InteractingCharacter);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interactive Object")
		void BP_Interact(AMFG_Character* InteractingCharacter);

	virtual void StopInteract(AMFG_Character* InteractingCharacter);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interactive Object")
		void BP_StopInteract(AMFG_Character* InteractingCharacter);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	void SetObjectActiveState(bool NewState);

};
