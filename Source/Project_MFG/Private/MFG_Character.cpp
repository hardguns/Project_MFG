// Fill out your copyright notice in the Description page of Project Settings.


#include "MFG_Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMFG_Character::AMFG_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseFirstPersonView = true;
	bIsCrouching = false;
	bIsRolling = false;
	bIsRunning = false;
	bIsUsingBag = false;
	Speed = 150.0f;
	FPSCameraSocketName = "SCK_Camera";

	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPS_CameraComponent"));
	FPSCameraComponent->bUsePawnControlRotation = true;
	FPSCameraComponent->SetupAttachment(GetMesh(), FPSCameraSocketName);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);

	TPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TPS_CameraComponent"));
	TPSCameraComponent->SetupAttachment(SpringArmComponent);

	//AnimInstance = CreateDefaultSubobject<UAnimInstance>(TEXT("BagAnimationInstance"));

	//CharacterMovementComponent = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("CharacterMovement"));
}

// Called when the game starts or when spawned
void AMFG_Character::BeginPlay()
{
	Super::BeginPlay();

}

void AMFG_Character::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector() * value);
}

void AMFG_Character::MoveRight(float value)
{
	AddMovementInput(GetActorRightVector() * value);
}

void AMFG_Character::CrouchStart()
{
	bIsCrouching = !bIsCrouching;
	if (bIsRunning)
	{
		bIsRunning = false;
	}

	if (bIsCrouching)
	{
		Super::Crouch();
	}
	else
	{
		Super::UnCrouch();
	}
}

//void AMFG_Character::CrouchEnd()
//{
//	bIsCrouching = false;
//	Super::UnCrouch();
//}

void AMFG_Character::RollStart()
{
	bIsRolling = true;
}

void AMFG_Character::RollEnd()
{
	bIsRolling = false;
}

void AMFG_Character::BagImpulse()
{
	bIsUsingBag = true;
	if (bIsUsingBag && !GetMovementComponent()->IsFalling())
	{
		FVector currentPosition;
		if (bUseFirstPersonView)
		{
			currentPosition = FPSCameraComponent->GetForwardVector();
			currentPosition.Z = 0.0f;
			//AnimInstance->Montage_Play()
			Super::LaunchCharacter(currentPosition * 10000.0f, false, true);
		}
		else
		{
			currentPosition = TPSCameraComponent->GetForwardVector();
			currentPosition.Z = 0.0f;
			Super::LaunchCharacter(currentPosition * 10000.0f, false, true);
		}
	}
}

void AMFG_Character::Run()
{
	bIsRunning = !bIsRunning;

	if (bIsCrouching)
	{
		Super::UnCrouch();
		bIsCrouching = false;
	}

	if (bIsRunning)
	{
		Speed = 900;
		GetCharacterMovement()->MaxWalkSpeed = Speed;
	}
	else
	{
		Speed = 600;
		GetCharacterMovement()->MaxWalkSpeed = Speed;
	}
}

void AMFG_Character::Jump()
{
	Super::Jump();
}

void AMFG_Character::StopJumping()
{
	Super::StopJumping();
}

void AMFG_Character::AddControllerPitchInput(float value)
{
	Super::AddControllerPitchInput(bIsLookInversion ? -value : value);
}

// Called every frame
void AMFG_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMFG_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMFG_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMFG_Character::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AMFG_Character::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &ACharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMFG_Character::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMFG_Character::StopJumping);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMFG_Character::CrouchStart);
	//PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMFG_Character::CrouchEnd);

	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &AMFG_Character::RollStart);
	PlayerInputComponent->BindAction("Roll", IE_Released, this, &AMFG_Character::RollEnd);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMFG_Character::Run);
	//PlayerInputComponent->BindAction("Run", IE_Released, this, &AMFG_Character::Run);

	PlayerInputComponent->BindAction("Impulse", IE_Pressed, this, &AMFG_Character::BagImpulse);
	//PlayerInputComponent->BindAction("Impulse", IE_Released, this, &AMFG_Character::BagImpulse);

}



