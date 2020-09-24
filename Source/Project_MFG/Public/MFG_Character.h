// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MFG_Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UCharacterMovementComponent;
class UAnimInstance;

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

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimInstance* AnimInstance;*/

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* BagAnimationMontage;*/

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCharacterMovementComponent* CharacterMovementComponent;*/

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		bool bIsRolling;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	bool bIsUsingBag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float Speed;

public:
	// Sets default values for this character's properties
	AMFG_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);

	void MoveRight(float value);

	void CrouchStart();

	void CrouchEnd();

	void RollStart();

	void RollEnd();

	void BagImpulse();

	void Run();

	virtual void Jump();

	virtual void StopJumping();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void AddControllerPitchInput(float value) override;

};
