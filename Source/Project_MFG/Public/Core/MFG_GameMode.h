// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MFG_GameMode.generated.h"

class AMFG_Character;
class AMFG_SpectatingCamera;
/**
 * 
 */
UCLASS()
class PROJECT_MFG_API AMFG_GameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spectating Camera")
		float SpectatingBlendTime;
		
	UPROPERTY(BlueprintReadOnly, Category = "Spectating Camera")
		AMFG_SpectatingCamera* VictoryCamera;

	UPROPERTY(BlueprintReadOnly, Category = "Spectating Camera")
		AMFG_SpectatingCamera* GameOverCamera;

protected:

	virtual void BeginPlay() override;

	void SetupSpectatingCameras();

	void MoveCameraToSpectatingPoint(AMFG_Character* Character, AMFG_SpectatingCamera* SpectatingCamera);

public: 
	
	UFUNCTION()
	void Victory(AMFG_Character* Character);

	UFUNCTION()
	void GameOver(AMFG_Character* Character);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_Victory(AMFG_Character* Character);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_GameOver(AMFG_Character* Character);
	
};
