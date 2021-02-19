// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MFG_HealerBot.generated.h"

class UStaticMeshComponent;
class UMFG_HealthComponent;
class AMFG_Enemy;
class AMFG_Character;
class UParticleSystem;
class USphereComponent;
class UParticleSystemComponent;
class UDecalComponent;
class AMFG_Shield;
class AMFG_Projectile;
class AMFG_Item;

UCLASS()
class PROJECT_MFG_API AMFG_HealerBot : public APawn
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* BotMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USphereComponent* HealDetectorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UMFG_HealthComponent* HealthComponent;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot")
		bool bDebug;

	UPROPERTY(BlueprintReadOnly, Category = "Bot Heal")
		bool bIsHealing;

	UPROPERTY(BlueprintReadOnly, Category = "Bot")
		bool bIsOverlapping;

	UPROPERTY(BlueprintReadOnly, Category = "Bot Attack")
		bool bIsAttacking;

	UPROPERTY(BlueprintReadOnly, Category = "Bot Heal")
		bool bHasToHeal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Heal")
		float SecondsToHeal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Movement")
		float MinDistanceToTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Movement")
		float ForceMagnitude;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Heal")
		float HealAmount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot")
		float ActionRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Heal")
		float HealCounter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bot Ultimate XP")
		float XPValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System")
		float LootProbability;

	UPROPERTY(BlueprintReadOnly, Category = "Bot")
		FVector NextPathPoint;

	UPROPERTY(BlueprintReadOnly, Category = "References")
		AMFG_Character* PlayerReference;

	UPROPERTY(BlueprintReadOnly, Category = "References")
		AMFG_Enemy* EnemyReference;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Effect")
		UDecalComponent* DecalEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Effect")
		UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Effect")
		UParticleSystem* HealingEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Heal")
		TArray<AActor*> ActorsToHeal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Heal")
		TArray<TEnumAsByte<EObjectTypeQuery>> ActorsEnum;

	UPROPERTY(EditDefaultsOnly, Category = "Bot Effect")
		UMaterialInterface* HealDecalMaterial;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Shield")
		FName ShieldSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
		TSubclassOf<AMFG_Shield> InitialShieldClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Attack")
		TSubclassOf<AMFG_Projectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot System")
		TSubclassOf<AMFG_Item> LootItemClass;

	UParticleSystemComponent* HealingAttachedEffectComponent;

	UMaterialInstanceDynamic* BotMaterial;

	UMaterialInstanceDynamic* DecalMaterial;

	FTimerHandle TimerHandle_HealTimer;

	FTimerHandle TimerHandle_BeginAttackBehaviour;

public:
	// Sets default values for this pawn's properties
	AMFG_HealerBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void TakingDamage(UMFG_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	FVector GetNextPathPoint();

	UFUNCTION()
	void StartAllyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndAllyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetBotBehaviour();

	void UnsetBotBehaviour();

	void HealAllies();

	void GiveShieldToAllies();

	void AttackPlayer();

	UFUNCTION()
	void GiveXP(AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_GiveXP(AActor* DamageCauser);

	bool TrySpawnLoot();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
