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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUltimateUpdateSignature, float, CurrentUltimateXP, float, MaxUltimateXP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUltimateStatusSignature, bool, bIsAvailable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityChangeSignature, int, AbilityAmountAvailable, int, ReceivedAbilityIndex);

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

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UMFG_HealthComponent* HealthComponent;
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
		bool bUseFirstPersonView;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
		float MeleeDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee", meta = (EditCondition = bCanMakeCombos, ClampMin = 1.0, UIMin = 1.0))
		float MaxComboMultiplier;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Time")
		float UltimateFrequency;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities", meta = (ClampMin = 0.0, UIMin = 0.0))
		float UltimatePlayRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities", meta = (ClampMin = 0.0, UIMin = 0.0))
		float WeaponInitialDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities", meta = (ClampMin = 0.0, UIMin = 0.0))
		float UltimateWeaponDamageMultiplier;

	UPROPERTY(BlueprintReadOnly, Category = "Ultimate|Abilities", meta = (ClampMin = 0.0, UIMin = 0.0))
		float PlayRate;

	UPROPERTY(BlueprintReadOnly, Category = "Melee", meta = (ClampMin = 0.0, UIMin = 0.0))
		float MeleePlayRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ultimate|Abilities", meta = (ClampMin = 0.0, UIMin = 0.0))
		float UltimateShotFrequency;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
		FName FPSCameraSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Melee")
		FName MeleeSocketName;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Key")
		TArray<FName> DoorKeys;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		EMFG_CharacterType CharacterType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<AMFG_Weapon> InitialWeaponClass;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
		AMFG_Weapon* CurrentWeapon;

	UPROPERTY(BlueprintReadWrite, Category = "Shield")
		AMFG_Shield* CurrentShield;

	UPROPERTY(BlueprintReadWrite, Category = "HUD")
		UMFG_HUD* CharacterMainHUD;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* MeleeMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* UltimateMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		UParticleSystem* BurningEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		UParticleSystemComponent* BurningEffectComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		UParticleSystem* UltimateWeaponEffect;

	UPROPERTY(BlueprintReadOnly, Category = "Effects")
		UParticleSystemComponent* UltimateWeaponEffectComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
		TArray<TSubclassOf<AMFG_Ability>> CharacterAbilitiesClasses;

	UPROPERTY(BlueprintReadOnly, Category = "Ability")
		TArray<AMFG_Ability*> CharacterAbilitiesArr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
		UTexture2D* CharacterHUDImage;

	UAnimInstance* MyAnimInstance;

	AMFG_GameMode* GameModeReference;

	UMFG_GameInstance* GameInstanceReference;

	FTimerHandle TimerHandle_Ultimate;

	FTimerHandle TimerHandle_AutomaticShot;

	FTimerHandle TimerHandle_BeginUltimateBehavior;

	FTimerHandle TimerHandle_ReloadAbility1;

	FTimerHandle TimerHandle_ReloadAbility2;

	FTimerDelegate TimerDelegate;

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

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	//	bool bIsWeaponAutomatic;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
		AMFG_InteractiveObject* InteractiveObject;

public:
	// Sets default values for this character's properties
	AMFG_Character();

	virtual FVector GetPawnViewLocation() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitializeReferences();

	void MoveForward(float value);

	void MoveRight(float value);

	void CrouchStart();

	void RollStart();

	void UsePrimaryAbility();

	UFUNCTION(BlueprintCallable)
	void Run();

	void StopRunning();

	void SetCharacterSpeed();

	void DoAction();

	virtual void Jump();

	virtual void StopJumping();

	void CreateInitialWeapon();

	void CreateInitialAbilities();

	UFUNCTION(BlueprintCallable)
	void StartWeaponAction();

	UFUNCTION(BlueprintCallable)
	void StopWeaponAction();

	void SetWeaponBehavior();

	UFUNCTION(BlueprintCallable)
	void StartMelee();

	void StopMelee();

	void StartAbility();

	void StopAbility();

	void StartUltimate();

	void StopUltimate();

	void GoToMainMenu();

	UFUNCTION()
	void MakeMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHealthChange(UMFG_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

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

	UTexture2D* GetCharacterHUDImage(){ return CharacterHUDImage; };

	void SetIsUsingBag(bool NewState);

	void SetMeleeDetectorCollision(ECollisionEnabled::Type NewCollisionState);

	void SetMeleeState(bool NewState);

	void SetAbilityState(bool NewState);

	void SetAbilityBehavior();

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

	TArray<AMFG_Ability*> GetCharacterAbilities() { return CharacterAbilitiesArr; };

protected:

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
