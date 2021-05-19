// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MFG_Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UCharacterMovementComponent;
class AMFG_InteractiveObject;
class AMFG_Weapon;
class UAnimMontage;
class UAnimInstance;
class UMFG_HealthComponent;
class UMFG_EffectsComponent;
class AMFG_GameMode;
class UParticleSystem;
class AMFG_LaserProjectile;
class AMFG_Shield;
class UMFG_GameInstance;
class AMFG_Ability;
class UMFG_HUD;
class UAudioComponent;
class USoundCue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUltimateUpdateSignature, float, CurrentUltimateXP, float, MaxUltimateXP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUltimateStatusSignature, bool, bIsAvailable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAbilityChangeSignature, int, AbilityAmountAvailable, int, ReceivedAbilityIndex, bool, bShowReloadingBar);

DECLARE_DELEGATE_OneParam(FStartAbilityDelegate, int);

UENUM()
enum class EMFG_CharacterType : uint8
{
	CharacterType_Player		UMETA(DisplayName = "Player"),
	CharacterType_Enemy			UMETA(DisplayName = "Enemy")
};

UCLASS()
class PROJECT_MFG_API AMFG_Character : public ACharacter
{
	GENERATED_BODY()

protected:

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCameraComponent* FPSCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCameraComponent* TPSCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UCapsuleComponent* MeleeDetectorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UMFG_EffectsComponent* EffectsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UAudioComponent* AbilitiesSoundComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UAudioComponent* StepSoundComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UAudioComponent* VoiceSoundComponent;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UMFG_HealthComponent* HealthComponent;
	
protected:

	//Sets if camera will be in first or third person view
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
		bool bUseFirstPersonView;

	//Sets if pitch (up and down) look will be inverted 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
		bool bIsLookInversion;

	UPROPERTY(BlueprintReadOnly, Category = "Melee")
		bool bIsDoingMelee;
		
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
		bool bCanUseWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
		bool bCanMakeCombos;

	UPROPERTY(BlueprintReadOnly, Category = "Melee")
		bool bIsComboEnable;

	UPROPERTY(BlueprintReadOnly, Category = "Game Over")
		bool bHasToDestroy;

	//Sets if ultimate will use Tick method
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate")
		bool bUltimateWithTick;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate")
		bool bCanUseUltimate;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate")
		bool bIsUsingUltimate;

	UPROPERTY(BlueprintReadOnly, Category = "Ability")
		bool bCanUseAbility;

	UPROPERTY(BlueprintReadOnly, Category = "Ability")
		bool bIsUsingAbility;
		
	UPROPERTY(BlueprintReadOnly, Category = "Audio")
		bool bWasDamaged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
		float MeleeDamage;

	//Sets maximum combo multiplier amount
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee", meta = (EditCondition = bCanMakeCombos, ClampMin = 1.0, UIMin = 1.0))
		float MaxComboMultiplier;

	//Sets current combo multiplier when using melee
	UPROPERTY(BlueprintReadOnly, Category = "Melee", meta = (EditCondition = bCanMakeCombos, ClampMin = 1.0, UIMin = 1.0))
		float CurrentComboMultiplier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = 0.0, UIMin = 0.0))
		float WalkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (ClampMin = 0.0, UIMin = 0.0))
		float RunSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities", meta = (ClampMin = 0.0, UIMin = 0.0))
		float UltimateWalkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities", meta = (ClampMin = 0.0, UIMin = 0.0))
		float UltimateRunSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate", meta = (ClampMin = 0.0, UIMin = 0.0))
		float MaxUltimateXP;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate")
		float CurrentUltimateXP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Time", meta = (ClampMin = 0.0, UIMin = 0.0))
		float MaxUltimateDuration;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate|Time")
		float CurrentUltimateDuration;

	//Updates ultimate counter when activated (This is used when using ultimate by timer only)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Time")
		float UltimateFrequency;

	//Sets animation play rate for ultimate montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities", meta = (ClampMin = 0.0, UIMin = 0.0))
		float UltimatePlayRate;

	//Sets initial damage for weapon
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities", meta = (ClampMin = 0.0, UIMin = 0.0))
		float WeaponInitialDamage;

	//Sets ultimate damage for weapon
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities", meta = (ClampMin = 0.0, UIMin = 0.0))
		float UltimateWeaponDamageMultiplier;

	//Sets global play rate for animations use in AnimationBlueprint
	UPROPERTY(BlueprintReadOnly, Category = "Ultimate|Abilities", meta = (ClampMin = 0.0, UIMin = 0.0))
		float PlayRate;

	//Sets play rate for melee animation montage
	UPROPERTY(BlueprintReadOnly, Category = "Melee", meta = (ClampMin = 0.0, UIMin = 0.0))
		float MeleePlayRate;

	//Sets how often a weapon can be used when ultimate is active
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities", meta = (ClampMin = 0.0, UIMin = 0.0))
		float UltimateShotFrequency;

	//Sets socket name to attach FPS camera to Character
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
		FName FPSCameraSocketName;

	//Sets socket name to attach melee detector component
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Melee")
		FName MeleeSocketName;

	//Sets socket name to attach particle systems
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		FName EffectsSocketName;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Levels")
		FName MainMenuMapName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		bool bIsCrouching;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		bool bIsRunning;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		bool bCanRoll;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		bool bIsRolling;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		bool bIsUsingBag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float DashForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float RollForce;

	//Sets how much time will take to have ultimate ready if it fills automatically
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate")
		float MaxUltimateReloadSeconds;
	
	//Saves all key names that have been picked up by player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
		TArray<FName> DoorKeys;

	//Enum to know which CharacterType has an actor
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		EMFG_CharacterType CharacterType;
	
	//Sets initial weapon that enemy or player will have 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<AMFG_Weapon> InitialWeaponClass;

	//Saves weapon instance
	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
		AMFG_Weapon* CurrentWeapon;

	//Saves shield instance if shield is given for enemies
	UPROPERTY(BlueprintReadWrite, Category = "Shield")
		AMFG_Shield* CurrentShield;

	//Sets player main HUD
	UPROPERTY(BlueprintReadWrite, Category = "HUD")
		UMFG_HUD* CharacterMainHUD;

	//Animation Montages
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* MeleeMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* UltimateMontage;

	//Effects
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		UParticleSystem* BurningEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		UParticleSystemComponent* BurningEffectComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		UParticleSystem* UltimateWeaponEffect;

	UPROPERTY(BlueprintReadOnly, Category = "Effects")
		UParticleSystemComponent* UltimateWeaponEffectComponent;

	//Sets which abilities class will have available player or enemy 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
		TArray<TSubclassOf<AMFG_Ability>> CharacterAbilitiesClasses;

	//Sets instance of abilities for player or enemy
	UPROPERTY(BlueprintReadOnly, Category = "Ability")
		TArray<AMFG_Ability*> CharacterAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
		UTexture2D* CharacterHUDImage;

	UAnimInstance* MyAnimInstance;

	AMFG_GameMode* GameModeReference;

	UMFG_GameInstance* GameInstanceReference;

	FTimerHandle TimerHandle_Ultimate;

	FTimerHandle TimerHandle_GainXP;

	FTimerHandle TimerHandle_AutomaticShot;

	FTimerHandle TimerHandle_BeginUltimateBehavior;

	FTimerDelegate TimerDelegate;

	//Sound Cues
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* HurtSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* AlmostDeadSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* FullHealthSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* WalkRunStepSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* RunningBreatheSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* BeginJumpSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* EndJumpSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* MeleeSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* DeathSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
		USoundCue* UltimateSound;

public:

	UPROPERTY(BlueprintAssignable)
		FOnUltimateUpdateSignature OnUltimateUpdateDelegate;

	UPROPERTY(BlueprintAssignable)
		FOnUltimateStatusSignature OnUltimateStatusDelegate;

	UPROPERTY(BlueprintAssignable)
		FOnAbilityChangeSignature OnAbilityChangeDelegate;
		
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
		bool bCanUseItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		bool bIsShooting;

	//When close to object interactive object will be set for player action
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
		AMFG_InteractiveObject* InteractiveObject;

public:
	// Sets default values for this character's properties
	AMFG_Character();

	virtual FVector GetPawnViewLocation() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Sets different references from map
	void InitializeReferences();

	//Called when player is moving forward or backward
	void MoveForward(float value);

	//Called when player is moving left or right
	void MoveRight(float value);

	//Called when player or enemy is crouching
	void CrouchStart();

	//Called when player is doing a roll
	void RollStart();

	//Called when player or enemy is running
	UFUNCTION(BlueprintCallable)
	void Run();

	//Called when player or enemy is trying to running
	void StopRunning();

	//Sets speed depending on player action (running, walking, crouching)
	void SetCharacterSpeed();

	//Interacts with object when player use action key
	void DoAction();

	virtual void Jump();

	virtual void StopJumping();

	//Spawns initial weapon actor to attach it to player or enemy
	void CreateInitialWeapon();

	//Spawns initial ability actors to attach them to player or enemy
	void CreateInitialAbilities();

	//When starting game or after using ultimate, this function reloads ultimate automatically
	void StartUltimateLoading();

	UFUNCTION(BlueprintCallable)
	void StartWeaponAction();

	UFUNCTION(BlueprintCallable)
	void StopWeaponAction();

	//Sets weapon behavior depending on the equipped weapon
	void SetWeaponBehavior();

	UFUNCTION(BlueprintCallable)
	void StartMelee();

	void StopMelee();

	//Receives index from player key actions to cast an ability
	void StartAbility(int index);

	//If some action cancels an ability StopAbility is used
	void StopAbility();

	void StartUltimate();

	void StopUltimate();

	void GoToMainMenu();

	void PlaySound(USoundCue* PlayableSound);

	//Delegates - subscriptions
	UFUNCTION()
	void MakeMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHealthChange(UMFG_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnHealthUpdate(float Health, float MaxHealth);

	UFUNCTION()
	void OnBurningStateChange(UMFG_EffectsComponent* CurrentEffectsComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void AddControllerPitchInput(float value) override;

	FVector GetCurrentPosition();

	void AddKey(FName NewKey);

	void RemoveKey(FName KeyToRemove);

	bool TryAddHealth(float HealthToAdd);

	bool HasKey(FName KeyTag);

	bool GetIsUsingBag(){ return bIsUsingBag; };

	float GetUltimateWeaponDamageMultiplier(){ return UltimateWeaponDamageMultiplier; };

	UMFG_HUD* GetCharacterMainHUD(){ return CharacterMainHUD; };

	UTexture2D* GetCharacterHUDImage(){ return CharacterHUDImage; };

	AMFG_InteractiveObject* GetInteractiveObject(){ return InteractiveObject; };

	void SetInteractiveObject(AMFG_InteractiveObject* NewInteractiveObject){ InteractiveObject = NewInteractiveObject; };

	void SetIsUsingBag(bool NewState);

	void SetMeleeDetectorCollision(ECollisionEnabled::Type NewCollisionState);

	void SetMeleeState(bool NewState);

	void SetAbilityState(bool NewState);

	void SetAbilityBehavior(int index);

	UFUNCTION(BlueprintCallable)
	void SetComboEnable(bool NewState);

	UFUNCTION(BlueprintCallable)
	void ResetCombo();

	bool HasToDestroy() { return bHasToDestroy; };

	bool GetIsUsingUltimate() { return bIsUsingUltimate; };

	float GetMultiplier() { return UltimateWeaponDamageMultiplier; };

	UFUNCTION(BlueprintCallable)
	void GainUltimateXP(float XPGained);

	void UpdateUltimateDuration(float Value);

	void UpdateUltimateDurationWithTimer();

	void BeginUltimateBehavior();

	UAnimInstance* GetPlayerAnimInstance() { return MyAnimInstance; };

	UFUNCTION(BlueprintCallable)
	EMFG_CharacterType GetCharacterType() { return CharacterType; };

	void SetShield(AMFG_Shield* NewShield);

	AMFG_Shield* GetShieldActor() { return CurrentShield; };

	UMFG_HealthComponent* GetHealthComponent(){ return HealthComponent; };

	TArray<AMFG_Ability*> GetCharacterAbilities() { return CharacterAbilities; };

	USoundCue* GetWalkRunSound() { return WalkRunStepSound; };

	USoundCue* GetEndJumpSound() { return EndJumpSound; };

	USoundCue* GetRunningBreatheSound() { return RunningBreatheSound; };

	void PlayStepSound(USoundCue* StepSound);

	void PlayAbilitySound(FName AbilityName);

	void PlayVoiceSound(USoundCue* VoiceSound);

protected:

	//Functions callable in blueprint as events 
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_GainUltimateXP(float XPGained);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_StartUltimate();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_StopUltimate();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void BP_StartAbility();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void BP_StopAbility();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_UpdateUltimateDuration(float Value);

};
