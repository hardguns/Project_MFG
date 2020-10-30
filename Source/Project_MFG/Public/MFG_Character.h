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
		UMFG_HealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UMFG_EffectsComponent* EffectsComponent;
	
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
		float MeleeDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee", meta = (EditCondition = bCanMakeCombos, ClampMin = 1.0, UIMin = 1.0))
		float MaxComboMultiplier;

	UPROPERTY(BlueprintReadOnly, Category = "Melee", meta = (EditCondition = bCanMakeCombos, ClampMin = 1.0, UIMin = 1.0))
		float CurrentComboMultiplier;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
		FName FPSCameraSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Melee")
		FName MeleeSocketName;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<AMFG_Weapon> InitialWeaponClass;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
		AMFG_Weapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* MeleeMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		UParticleSystemComponent* BurningEffectComponent;

	UAnimInstance* MyAnimInstance;

	AMFG_GameMode* GameModeReference;

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

	void BagImpulse();

	void Run();

	void StopRunning();

	void SetCharacterSpeed();

	void DoAction();

	virtual void Jump();

	virtual void StopJumping();

	void CreateInitialWeapon();

	void StartWeaponAction();

	void StopWeaponAction();

	void SetWeaponBehavior();

	void StartMelee();

	void StopMelee();

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

	bool HasKey(FName KeyTag);

	void SetMeleeDetectorCollision(ECollisionEnabled::Type NewCollisionState);

	void SetMeleeState(bool NewState);

	UFUNCTION(BlueprintCallable)
	void SetComboEnable(bool NewState);

	UFUNCTION(BlueprintCallable)
	void ResetCombo();

	bool HasToDestroy() { return bHasToDestroy; };

};
