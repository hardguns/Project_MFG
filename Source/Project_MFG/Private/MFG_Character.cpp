// Fill out your copyright notice in the Description page of Project Settings.


#include "MFG_Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Objects/MFG_InteractiveObject.h"
#include "Objects/MFG_Activable.h"
#include "Weapons/MFG_Weapon.h"
#include "Weapons/MFG_Rifle.h"

// Sets default values
AMFG_Character::AMFG_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseFirstPersonView = true;
	bIsCrouching = false;
	bCanRoll = true;
	bIsRolling = false;
	bIsRunning = false;
	bIsUsingBag = false;
	Speed = 150.0f;
	DashForce = 10000.0f;
	RollForce = 15000.0f;
	FPSCameraSocketName = "SCK_Camera";

	bCanUseItem = false;

	//bIsWeaponAutomatic = false;

	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FPS_CameraComponent"));
	FPSCameraComponent->bUsePawnControlRotation = true;
	FPSCameraComponent->SetupAttachment(GetMesh(), FPSCameraSocketName);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);

	TPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TPS_CameraComponent"));
	TPSCameraComponent->SetupAttachment(SpringArmComponent);

	InteractiveObject = NULL;

}

FVector AMFG_Character::GetPawnViewLocation() const
{
	if (IsValid(FPSCameraComponent) && bUseFirstPersonView)
	{
		return FPSCameraComponent->GetComponentLocation();
	}

	if (IsValid(TPSCameraComponent) && !bUseFirstPersonView)
	{
		return TPSCameraComponent->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();
}

// Called when the game starts or when spawned
void AMFG_Character::BeginPlay()
{
	Super::BeginPlay();

	CreateInitialWeapon();

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
		Super:: Crouch();
	}
	else
	{
		Super::UnCrouch();
		SetCharacterSpeed();
	}
}

void AMFG_Character::RollStart()
{
	bIsRolling = true;
	/*if (bIsRolling)
	{
		FVector currentPosition = GetCurrentPosition();
		Super::LaunchCharacter(currentPosition * RollForce, true, true);
	}*/
}

//void AMFG_Character::RollEnd()
//{
//	bIsRolling = false;
//}

void AMFG_Character::BagImpulse()
{
	if (!bIsUsingBag && !GetMovementComponent()->IsFalling())
	{
		bIsUsingBag = true;
		FVector currentPosition = GetCurrentPosition();
		Super::LaunchCharacter(currentPosition * DashForce, true, true);
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

	SetCharacterSpeed();
}

void AMFG_Character::SetCharacterSpeed()
{
	if (bIsRunning)
	{
		Speed = 1300;
		GetCharacterMovement()->MaxWalkSpeed = Speed;
	}
	else
	{
		Speed = 600;
		GetCharacterMovement()->MaxWalkSpeed = Speed;
	}
}

void AMFG_Character::DoAction()
{
	if (IsValid(InteractiveObject))
	{
		AMFG_Activable* ActivableObj = Cast<AMFG_Activable>(InteractiveObject);
		if (IsValid(ActivableObj))
		{
			ActivableObj->UseActivable();
		}
	}

}

FVector AMFG_Character::GetCurrentPosition()
{
	FVector currentPosition;
	if (bUseFirstPersonView)
	{
		currentPosition = FPSCameraComponent->GetForwardVector();
		currentPosition.Z = 0.0f;
	}
	else
	{
		currentPosition = TPSCameraComponent->GetForwardVector();
		currentPosition.Z = 0.0f;
	}

	return currentPosition;
}

void AMFG_Character::Jump()
{
	Super::Jump();
}

void AMFG_Character::StopJumping()
{
	Super::StopJumping();
}

void AMFG_Character::CreateInitialWeapon()
{
	if (IsValid(InitialWeaponClass))
	{
		CurrentWeapon = GetWorld()->SpawnActor<AMFG_Weapon>(InitialWeaponClass, GetActorLocation(), GetActorRotation());
		if (IsValid(CurrentWeapon))
		{
			CurrentWeapon->SetCharacterOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}
}

void AMFG_Character::StartWeaponAction()
{
	bIsShooting = true;
	bIsRunning = false;

	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->StartAction();
	}
}

void AMFG_Character::StopWeaponAction()
{
	bIsShooting = false;
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->StopAction();
	}
}

void AMFG_Character::SetWeaponBehavior()
{
	AMFG_Rifle* RifleEquipped = Cast<AMFG_Rifle>(CurrentWeapon);
	if (IsValid(RifleEquipped))
	{
		RifleEquipped->bIsAutomatic = !RifleEquipped->bIsAutomatic;
		//bIsWeaponAutomatic = !bIsWeaponAutomatic;
	}
}

void AMFG_Character::AddControllerPitchInput(float value)
{
	Super::AddControllerPitchInput(bIsLookInversion ? -value : value);
}

// Called every frame
void AMFG_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (bIsShooting && bIsWeaponAutomatic)
	//{
	//	StartWeaponAction();
	//}

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

	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &AMFG_Character::RollStart);
	//PlayerInputComponent->BindAction("Roll", IE_Released, this, &AMFG_Character::RollEnd);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMFG_Character::Run);

	PlayerInputComponent->BindAction("Impulse", IE_Pressed, this, &AMFG_Character::BagImpulse);

	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &AMFG_Character::DoAction);

	PlayerInputComponent->BindAction("WeaponAction", IE_Pressed, this, &AMFG_Character::StartWeaponAction);
	PlayerInputComponent->BindAction("WeaponAction", IE_Released, this, &AMFG_Character::StopWeaponAction);
	
	PlayerInputComponent->BindAction("WeaponBehavior", IE_Pressed, this, &AMFG_Character::SetWeaponBehavior);

}

void AMFG_Character::AddKey(FName NewKey)
{
	DoorKeys.Add(NewKey);
}

bool AMFG_Character::HasKey(FName KeyTag)
{
	return DoorKeys.Contains(KeyTag);
}



