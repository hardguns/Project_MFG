// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MFG_Activator.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UPointLightComponent;
class UBoxComponent;

UCLASS()
class PROJECT_MFG_API AMFG_Activator : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USceneComponent* CustomRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* ActivatorMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UPointLightComponent* PointLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UBoxComponent* ActiveZoneColliderComponent;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Light")
		float LightIntensity;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Light")
		bool bSwitchState;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Activator")
		FName ActivatorTag;
	
public:	
	// Sets default values for this actor's properties
	AMFG_Activator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void CheckActivatorUse(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UseActivator();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "My Door")
		void BP_UseActivator();

};
