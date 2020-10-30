// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MFG_EffectsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnBurningStateChangeSignature, UMFG_EffectsComponent*, EffectsComponent, AActor*, DamagedActor, float, Damage, const UDamageType*, DamageType, AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS( ClassGroup=(MFG), meta=(BlueprintSpawnableComponent) )
class PROJECT_MFG_API UMFG_EffectsComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
		bool bDebug;

	UPROPERTY(BlueprintReadOnly, Category = "Effects Component")
		bool bIsBurning;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Effects Component")
		float TimeBetweenDamage;

	UPROPERTY(BlueprintReadOnly, Category = "Effects Component")
		int DamageSecondCounter;

	UPROPERTY(BlueprintReadOnly, Category = "Effects Component")
		AActor* MyOwner;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		TSubclassOf<UDamageType> DamageDealType;

	FTimerHandle TimerHandle_HandleEffects;

	FTimerDelegate TimerDelegate;

public:	
	// Sets default values for this component's properties
	UMFG_EffectsComponent();

public:

	UPROPERTY(BlueprintAssignable)
		FOnBurningStateChangeSignature OnBurningStateChangeDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void ApplyBurningDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void MakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

public:	

	bool IsBurning() { return bIsBurning; };
		
};
