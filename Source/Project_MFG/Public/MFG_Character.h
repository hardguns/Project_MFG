// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MFG_Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UCharacterMovementComponent;
class AMFG_Activator;
class AMFG_Weapon;

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
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
		bool bUseFirstPersonView;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
		bool bIsLookInversion;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aiming")
		FName FPSCameraSocketName;

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

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
		bool bCanUseItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		bool bIsShooting;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	//	bool bIsWeaponAutomatic;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Action")
		AMFG_Activator* InteractiveObject;

public:
	// Sets default values for this character's properties
	AMFG_Character();

	virtual FVector GetPawnViewLocation() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);

	void MoveRight(float value);

	void CrouchStart();

	void RollStart();

	void BagImpulse();

	void Run();

	void SetCharacterSpeed();

	void DoAction();

	virtual void Jump();

	virtual void StopJumping();

	void CreateInitialWeapon();

	void StartWeaponAction();

	void StopWeaponAction();

	void SetWeaponBehavior();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void AddControllerPitchInput(float value) override;

	FVector GetCurrentPosition();

	void AddKey(FName NewKey);

	bool HasKey(FName KeyTag);

};
