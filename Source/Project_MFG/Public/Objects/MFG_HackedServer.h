// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objects/MFG_Activable.h"
#include "MFG_HackedServer.generated.h"

class UMFG_HealthComponent;
class UWidgetComponent;
class UMFG_EnemyHealthBar;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartHackSignature, float, HackingTime);

/**
 * 
 */
UCLASS()
class PROJECT_MFG_API AMFG_HackedServer : public AMFG_Activable
{
	GENERATED_BODY()

protected:

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UMFG_HealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UWidgetComponent* WidgetHealthBarComponent;

public:

	//Triggers when player has started hacking server
	UPROPERTY(BlueprintAssignable)
		FOnStartHackSignature OnStartHackingDelegate;

protected:
	
	UPROPERTY(BlueprintReadOnly, Category = "Server")
		bool bWasHacked;

	//Sets if health is currently showing
	UPROPERTY(BlueprintReadOnly, Category = "Server")
		bool bIsShowingHealthBar;

	//Sets how much time takes hacking the server
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Server")
		float HackingTime;

	//HealthBar widget reference
	UPROPERTY(BlueprintReadOnly, Category = "UI")
		UMFG_EnemyHealthBar* ServerHealthBar;

	FTimerHandle TimerHandle_HideHealthBar;

	FTimerHandle TimerHandle_DownloadInformation;

public:

	AMFG_HackedServer();

protected:

	virtual void BeginPlay() override;
	
	virtual void CheckActivable(AMFG_Character* OtherActor) override;

	//Sets behavior when activating server hacking
	virtual void UseActivable() override;

	//Triggers action when server health in changed
	UFUNCTION()
	void OnHealthChange(UMFG_HealthComponent* ServerHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	//Sets behavior after server was hacked
	void FinishHacking();

	void ShowHealthBar();

	void HideHealthBar();
	
};
