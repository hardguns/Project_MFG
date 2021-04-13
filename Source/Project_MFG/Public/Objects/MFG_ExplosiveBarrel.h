// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MFG_ExplosiveBarrel.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UCapsuleComponent;
class UMFG_HealthComponent;
class UParticleSystem;
class USphereComponent;
class USoundCue;

UCLASS()
class PROJECT_MFG_API AMFG_ExplosiveBarrel : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USceneComponent* CustomRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* BarrelMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCapsuleComponent* BarrelColliderComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USphereComponent* DamageColliderAreaComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UMFG_HealthComponent* HealthComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
		float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
		float DamageRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		float TimeToExplode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		UParticleSystem* BurningEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		TSubclassOf<UDamageType> DamageDealType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		TArray<AActor*> ActorsToDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		TArray<TEnumAsByte<EObjectTypeQuery>> ActorsEnum;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* ExplosionSound;

	FTimerHandle TimerHandle_HandleExplosion;

public:	
	// Sets default values for this actor's properties
	AMFG_ExplosiveBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHealthChange(UMFG_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float DamageReceived, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	void Explode();

	void PlayExplosionSound();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
