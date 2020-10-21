// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MFG_SpectatingCamera.generated.h"

class UStaticMeshComponent;

UENUM()
enum class EMFG_SpectatingCameraType : uint8 
{
	CameraType_None		UMETA(DisplayName = "None"),
	CameraType_Victory	UMETA(DisplayName = "Victory"),
	CameraType_GameOver	UMETA(DisplayName = "Game Over")
};

UCLASS()
class PROJECT_MFG_API AMFG_SpectatingCamera : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* SpectatingCameraComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spectating Camera")
		EMFG_SpectatingCameraType CameraType;
	
public:	
	// Sets default values for this actor's properties
	AMFG_SpectatingCamera();

	EMFG_SpectatingCameraType GetCameraType() { return CameraType; };

};
