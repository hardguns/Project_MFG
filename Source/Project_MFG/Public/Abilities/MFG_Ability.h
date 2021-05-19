// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MFG_Ability.generated.h"

class AMFG_Character;
class USoundCue;

USTRUCT(BlueprintType)
struct FMFG_AbilityStruct
{
	GENERATED_USTRUCT_BODY()

public:

	//Sets ability position in abilities array in character or enemy
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int AbilityIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName AbilityName;

	//Sets maximum times that an ability can be used
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int MaximumAbilityUseAmount;

	//Sets current times that an ability can be used
	UPROPERTY(BlueprintReadOnly)
		int CurrentAbilityUseAmount;
	
	//Sets ability reloading time
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float AbilityCooldown;

	//Sets a 2Dtexture that will be shown in main HUD
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UTexture2D* AbilityIcon;

	//Sets a sound that will be played when an ability is used
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		USoundCue* AbilitySound;

public:
	
	USoundCue* GetAbilitySound() { return AbilitySound; };

};

UCLASS()
class PROJECT_MFG_API AMFG_Ability : public AActor
{
	GENERATED_BODY()

protected:

	//Struct to create a new ability
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Setup")
		FMFG_AbilityStruct AbilityDetails;

	UPROPERTY(BlueprintReadOnly, Category = "Ability Setup")
		AMFG_Character* PlayerCharacterReference;

	FTimerHandle TimerHandle_ReloadAbility;

public:	
	// Sets default values for this actor's properties
	AMFG_Ability();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void CastAbility(AMFG_Character* AbilityCaster);

	virtual void SetAbilityBehavior(AMFG_Character* AbilityCaster);

	virtual void StopCastingAbility();

	virtual void ReloadAbility();

protected:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void BP_CastAbility(AMFG_Character* AbilityCaster);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void BP_SetAbilityBehavior(AMFG_Character* AbilityCaster);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void BP_StopCastingAbility();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void BP_ReloadAbility();

public:

	FMFG_AbilityStruct GetAbilityDetails(){ return AbilityDetails; };

};
