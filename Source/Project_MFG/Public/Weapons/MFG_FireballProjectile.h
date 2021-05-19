// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/MFG_Projectile.h"
#include "MFG_FireballProjectile.generated.h"

class UParticleSystemComponent;
class UParticleSystem;
class AMFG_Character;
/**
 * 
 */
UCLASS()
class PROJECT_MFG_API AMFG_FireballProjectile : public AMFG_Projectile
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
		UParticleSystemComponent* FireballEffectComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UMFG_HealthComponent* FireballHealthComponent;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Destination")
		float HomingStrength;

	UPROPERTY(BlueprintReadOnly, Category = "Destination")
		bool bCanMove;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		UParticleSystem* SpawnEffect;

	UPROPERTY(BlueprintReadOnly, Category = "Destination")
		AMFG_Character* TargetCharacter;
	
	FTimerHandle TimerHandle_ReachDestination;

public:

	AMFG_FireballProjectile();

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void ReachDestination(float DeltaTime);

	UFUNCTION()
	void OnHealthChange(UMFG_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float DamageDealed, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

public: 

	void SetTargetCharacter(AMFG_Character* NewTarget) { TargetCharacter = NewTarget; };

};
