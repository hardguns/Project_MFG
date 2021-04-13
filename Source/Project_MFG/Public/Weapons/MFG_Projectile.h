// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MFG_Projectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UParticleSystem;
class USoundCue;

UCLASS()
class PROJECT_MFG_API AMFG_Projectile : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USphereComponent* ProjectileCollision;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* ProjectileMesh; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UProjectileMovementComponent* ProjectileMovementComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
		float DamageRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
		float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* ExplosionSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		TSubclassOf<UDamageType> DamageType;

	FTimerHandle TimerHandle_HandleExplosion;

public:	
	// Sets default values for this actor's properties
	AMFG_Projectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnDetonate();

	void PlayExplosionSound();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	float GetDamageValue(){ return Damage; };

	void SetNewDamageValue(float NewDamage);
};
