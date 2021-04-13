// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MFG_ProximityMine.generated.h"

class UStaticMeshComponent;
class USceneComponent;
class USphereComponent;
class UMFG_HealthComponent;
class UParticleSystem;
class ACharacter;
class UPointLightComponent;
class USoundCue;

UCLASS()
class PROJECT_MFG_API AMFG_ProximityMine : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USceneComponent* CustomRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* MineMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USphereComponent* SphereColliderComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UPointLightComponent* MineLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UMFG_HealthComponent* HealthComponent;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		bool bDebug;

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
		float CharacterDistance;

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
		float DetonationDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
		float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
		float DamageRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Light")
		float LightIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
		float XPValue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		TSubclassOf<UDamageType> DamageDealType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Actor")
		ACharacter* CurrentActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		TArray<AActor*> ActorsToDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		TArray<TEnumAsByte<EObjectTypeQuery>> ActorsEnum;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* ProximitySound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* ExplosionSound;
	
public:	
	// Sets default values for this actor's properties
	AMFG_ProximityMine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Explode();

	void ValidateDistance();

	UFUNCTION()
	void EnterActivationZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ExitActivationZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnHealthChange(UMFG_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float DamageReceived, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	void PlayExplosionSound();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
