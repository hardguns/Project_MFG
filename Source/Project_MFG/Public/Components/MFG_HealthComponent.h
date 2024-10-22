// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MFG_HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangeSignature, UMFG_HealthComponent*, HealthComponent, AActor*, DamagedActor, float, Damage, const UDamageType*, DamageType, AController*, InstigatedBy, AActor*, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeadSignature, AActor*, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthUpdateSignature, float, CurrentHealth, float, MaxHealth);

UCLASS( ClassGroup=(MFG), meta=(BlueprintSpawnableComponent) )
class PROJECT_MFG_API UMFG_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug")
		bool bDebug;

	UPROPERTY(BlueprintReadOnly, Category = "Health Component")
		bool bIsDead;

	UPROPERTY(BlueprintReadWrite, Category = "Health Component")
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Component", meta = (ClampMin = 0.0, UIMin = 0.0))
		float MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Health Component")
		AActor* MyOwner;

	FTimerHandle TimerHandle_UpdateInitialHealth;

public:

	// Sets default values for this component's properties
	UMFG_HealthComponent();

	UFUNCTION(BlueprintCallable)
	bool IsDead() const { return bIsDead; };

	UFUNCTION()
	float GetCurrentHealth() { return Health; };

	UFUNCTION()
	float GetMaxHealth() { return MaxHealth; };

	bool TryAddHealth(float HealthToAdd);

	void UpdateInitialHealth();

public:

	UPROPERTY(BlueprintAssignable)
		FOnHealthChangeSignature OnHealthChangeDelegate;

	UPROPERTY(BlueprintAssignable)
		FOnDeadSignature OnDeadDelegate;

	UPROPERTY(BlueprintAssignable)
		FOnHealthUpdateSignature OnHealthUpdateDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void TakingDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
};
