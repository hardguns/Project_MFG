// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MFG_Bot.generated.h"

class UStaticMeshComponent;
class AMFG_Character;
class UMFG_HealthComponent;

UCLASS()
class PROJECT_MFG_API AMFG_Bot : public APawn
{
	GENERATED_BODY()

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* BotMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UMFG_HealthComponent* HealthComponent;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Movement")
		float MinDistanceToTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Movement")
		float ForceMagnitude;
	
	UPROPERTY(BlueprintReadOnly, Category = "Bot")
		FVector NextPathPoint;

	UPROPERTY(BlueprintReadOnly, Category = "References")
		AMFG_Character* PlayerCharacter;

public:
	// Sets default values for this pawn's properties
	AMFG_Bot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	FVector GetNextPathPoint();

	UFUNCTION()
	void TakingDamage(UMFG_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
