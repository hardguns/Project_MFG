// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MFG_LevelMusic.generated.h"

class UAudioComponent;
class UBillboardComponent;
class AMFG_GameMode;

UCLASS()
class PROJECT_MFG_API AMFG_LevelMusic : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMFG_LevelMusic();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UBillboardComponent* MusicBillboardComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UAudioComponent* MusicAudioComponent;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Music")
		FName AlertParamName;
	
	UPROPERTY(BlueprintReadOnly, Category = "References")
		AMFG_GameMode* GameModeReference;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void StopLevelMusic();

	UFUNCTION()
	void ChangeLevelMusic(bool bIsAlert);

};
