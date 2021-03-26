// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MFG_Ability.generated.h"

USTRUCT(BlueprintType)
struct FAbility
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int AbilityIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FName AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int MaximumAbilityUseAmount;

	UPROPERTY(BlueprintReadOnly)
		int CurrentAbilityUseAmount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float AbilityCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UTexture2D* AbilityIcon;
};

class AMFG_Character;

UCLASS()
class PROJECT_MFG_API AMFG_Ability : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Setup")
		FAbility AbilityDetails;

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

	virtual void CastAbility();

	virtual void SetAbilityBehavior();

	virtual void StopCastingAbility();

	virtual void ReloadAbility();

protected:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void BP_CastAbility();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void BP_SetAbilityBehavior();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void BP_StopCastingAbility();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void BP_ReloadAbility();

public:

	FAbility GetAbilityDetails(){ return AbilityDetails; };

};
