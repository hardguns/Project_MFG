// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MFG_Shield.generated.h"

class UStaticMeshComponent;
class UMFG_HealthComponent;
class AMFG_Character;
class UCapsuleComponent;

UCLASS()
class PROJECT_MFG_API AMFG_Shield : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
		UStaticMeshComponent* ShieldMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
		UCapsuleComponent* HitCapsuleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Component")
		UMFG_HealthComponent* HealthComponent;

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shield Colors")
	TArray<FLinearColor> ShieldColors;

	float HitCounter;

	AMFG_Character* ShieldOwner;

	UMaterialInstanceDynamic* ShieldMaterial;
	
public:	
	// Sets default values for this actor's properties
	AMFG_Shield();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void TakingDamage(UMFG_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetShieldOwner(AMFG_Character* CurrentShieldOwner);

	void DestroyShield();

};
