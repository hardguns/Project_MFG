// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MFG_Door.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UBoxComponent;
class UAudioComponent;
class USoundCue;

UCLASS()
class PROJECT_MFG_API AMFG_Door : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USceneComponent* CustomRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* DoorFrameComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* DoorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UBoxComponent* KeyZoneColliderComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UAudioComponent* DoorAudioComponent;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Door")
		float OpenAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Door")
		bool bIsOpen;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		FName DoorParamName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Door")
		FName DoorTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* DoorBehaviorSound;

public:	
	// Sets default values for this actor's properties
	AMFG_Door();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void CheckKeyFromPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OpenDoor();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "My Door")
	void BP_OpenDoor();

	void ChangeDoorSound();

	void PlayDoorSound(USoundCue* DoorSound);

};
