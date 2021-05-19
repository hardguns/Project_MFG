// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MFG_GameMode.generated.h"

class AMFG_Character;
class AMFG_SpectatingCamera;
class USoundCue;
class AMFG_Enemy;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKeyAddedSignature, FName, KeyTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnergyEnabledSignature, FName, ObjectiveIdentifier);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStateChange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAlertModeChangeSignature, bool, bIsAlert);

UCLASS()
class PROJECT_MFG_API AMFG_GameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Level")
		bool bIsAlertMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spectating Camera")
		float SpectatingBlendTime;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Flow")
		FName MainMenuMapName;
		
	UPROPERTY(BlueprintReadOnly, Category = "Spectating Camera")
		AMFG_SpectatingCamera* VictoryCamera;

	UPROPERTY(BlueprintReadOnly, Category = "Spectating Camera")
		AMFG_SpectatingCamera* GameOverCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Music")
		USoundCue* VictoryMusic;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Music")
		USoundCue* GameOverMusic;

	UPROPERTY(BlueprintReadOnly, Category = "Level")
		TArray<AMFG_Enemy*> LevelEnemies;

	FTimerHandle TimerHandle_BackToMainMenu;

public:

	AMFG_GameMode();

	UPROPERTY(BlueprintAssignable)
		FOnKeyAddedSignature OnKeyAddedDelegate; 
		
	UPROPERTY(BlueprintAssignable)
		FOnGameStateChange OnVictoryDelegate;

	UPROPERTY(BlueprintAssignable)
		FOnGameStateChange OnGameOverDelegate;
		
	UPROPERTY(BlueprintAssignable)
		FOnAlertModeChangeSignature OnAlertModeChangeDelegate;
		
	UPROPERTY(BlueprintAssignable)
		FOnEnergyEnabledSignature OnEnergyEnabledDelegate;

protected:

	virtual void BeginPlay() override;

	void SetupSpectatingCameras();

	void MoveCameraToSpectatingPoint(AMFG_Character* Character, AMFG_SpectatingCamera* SpectatingCamera);

	void PlayMusic(USoundCue* MusicCue);

public: 

	UFUNCTION()
	void AddKeyToCharacter(AMFG_Character* KeyOwner, FName KeyTag);

	UFUNCTION()
	void EnableElectricity(FName ObjectiveIdentier);
	
	UFUNCTION()
	void Victory(AMFG_Character* Character);

	UFUNCTION()
	void GameOver(AMFG_Character* Character);

	void BackToMainMenu();

	void CheckAlertMode();

	void AddEnemyToLevel(AMFG_Enemy* NewEnemy);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_Victory(AMFG_Character* Character);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_GameOver(AMFG_Character* Character);

};
