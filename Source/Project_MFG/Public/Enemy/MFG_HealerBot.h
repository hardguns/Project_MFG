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
class UMFG_GameInstance;
class USoundCue;

UCLASS()
class PROJECT_MFG_API AMFG_HealerBot : public APawn
{
	GENERATED_BODY()

public:

	//Bot components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* BotMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USphereComponent* HealDetectorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UMFG_HealthComponent* HealthComponent;

protected:

	//Sets behavior of debugging (for logs and drawDebugHelpers)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot")
		bool bDebug;

	//Bot states
	UPROPERTY(BlueprintReadOnly, Category = "Bot Heal")
		bool bIsHealing;

	UPROPERTY(BlueprintReadOnly, Category = "Bot Attack")
		bool bIsAttacking;

	//If an enemy needs to be heal will be active (Checks if bot has to heal or give shield)
	UPROPERTY(BlueprintReadOnly, Category = "Bot Heal")
		bool bHasToHeal;

	//Time (in seconds) for bot to know how often enemies are healed
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Heal")
		float TimeBetweenHealing;

	//Time (in seconds) for bot to know how much time it has to heal enemies
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Heal")
		float MaxTimeHealing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Movement")
		float MinDistanceToTarget;

	//Force that bot will have when going to a path point
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Movement")
		float ForceMagnitude;

	//Amount that bot will heal in certain time (watch TimeBetweenHealing and MaxTimeHealing variables)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Heal")
		float HealAmount;

	//Sets the radius in which bot will heal or give shield to enemies
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot")
		float ActionRadius;

	//Sets every time bot heals, it is used to stop bot healing
	UPROPERTY(BlueprintReadOnly, Category = "Bot Heal")
		float HealCounter;

	//XP Amount given when a bot is killed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bot Ultimate XP")
		float XPValue;

	//Probability amount to know if bot will give loot as reward
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot System")
		float LootProbability;

	//Bot next position to move
	UPROPERTY(BlueprintReadOnly, Category = "Bot")
		FVector NextPathPoint;

	UPROPERTY(BlueprintReadOnly, Category = "References")
		AMFG_Character* PlayerReference;

	//Sets an enemy reference to create a possible route of path points 
	UPROPERTY(BlueprintReadOnly, Category = "References")
		AMFG_Enemy* EnemyReference;

	//Effect to show what bot is doing at the moment
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Effect")
		UDecalComponent* DecalEffect;

	//Effect that will be active when bot gets killed
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Effect")
		UParticleSystem* ExplosionEffect;

	//Effect that will be active when bot is healing
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Effect")
		UParticleSystem* HealingEffect;

	//Sets actors to be healed when bot is healing
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Heal")
		TArray<AActor*> ActorsToHeal;

	//Sets the object types that will be have into account when making a sphere overlap
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Heal")
		TArray<TEnumAsByte<EObjectTypeQuery>> ActorsEnum;

	//Sets shield socket name to attach shield when bot is giving shield to enemies
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Shield")
		FName ShieldSocketName;

	//Shield class to spawn actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield")
		TSubclassOf<AMFG_Shield> InitialShieldClass;

	//Projectile class to spawn when bot is attacking
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bot Attack")
		TSubclassOf<AMFG_Projectile> ProjectileClass;

	//Item class to spawn at the moment of giving loot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot System")
		TSubclassOf<AMFG_Item> LootItemClass;

	//SoundCue to play when bot gets killed
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* ExplosionSound;

	//Effect that will be active when bot is healing
	UParticleSystemComponent* HealingAttachedEffectComponent;

	//Bot material is created to show when bot is taking damage
	UMaterialInstanceDynamic* BotMaterial;

	//Decal material is used to show when bot is healing or giving shield
	UMaterialInstanceDynamic* DecalMaterial;

	UMFG_GameInstance* GameInstanceReference;

	FTimerHandle TimerHandle_HealTimer;

	FTimerHandle TimerHandle_BeginAttackBehaviour;

public:
	// Sets default values for this pawn's properties
	AMFG_HealerBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Function use when bot is taking any kind of damage
	UFUNCTION()
	void TakingDamage(UMFG_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	//Gets bot's next path point to go to
	FVector GetNextPathPoint();

	//This action is triggered when bot is overlapping
	UFUNCTION()
	void StartAllyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Sets if bot has to heal or has to give shield to its "allies" 
	void SetBotBehaviour();

	//Unsets any bot behaviour (healing or giving shield)
	void UnsetBotBehaviour();

	void HealAllies();

	//Heal will stop when heal counter is greater or equals than MaxTimeHealing variable
	void TryStopHealing();

	void GiveShieldToAllies();

	void AttackPlayer();

	UFUNCTION()
	void GiveXP(AActor* DamageCauser);

	UFUNCTION(BlueprintImplementableEvent)
	void BP_GiveXP(AActor* DamageCauser);

	bool TrySpawnLoot();

	void PlayExplosionSound();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
