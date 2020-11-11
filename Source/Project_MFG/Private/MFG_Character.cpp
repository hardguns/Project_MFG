// Fill out your copyright notice in the Description page of Project Settings.


#include "MFG_Character.h"
#include "Project_MFG/Project_MFG.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Objects/MFG_InteractiveObject.h"
#include "Objects/MFG_ElectricityGen.h"
#include "Objects/MFG_Activable.h"
#include "Weapons/MFG_Weapon.h"
#include "Weapons/MFG_Rifle.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/MFG_HealthComponent.h"
#include "Components/MFG_EffectsComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Core/MFG_GameMode.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/ActorComponent.h"
#include "Weapons/MFG_LaserProjectile.h"

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
	bCanUseWeapon = true;
	Speed = 150.0f;
	WalkSpeed = 600.0f;
	RunSpeed = 1300.0f;
	DashForce = 10000.0f;
	RollForce = 15000.0f;
	FPSCameraSocketName = "SCK_Camera";
	MeleeSocketName = "SCK_Melee";
	EffectsSocketName = "Aim_Target";
	MeleeDamage = 10.0f;
	MeleePlayRate = 1.2f;
	MaxComboMultiplier = 4.0f;
	CurrentComboMultiplier = 1.0f;

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

	MeleeDetectorComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeDetectorComponent"));
	MeleeDetectorComponent->SetupAttachment(GetMesh(), MeleeSocketName);
	MeleeDetectorComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeleeDetectorComponent->SetCollisionResponseToChannel(COLLISION_ENEMY, ECR_Overlap);
	MeleeDetectorComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BurningEffectComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BurningEffectComponent"));
	BurningEffectComponent->SetupAttachment(GetCapsuleComponent());

	HealthComponent = CreateDefaultSubobject<UMFG_HealthComponent>(TEXT("HealthComponent"));

	EffectsComponent = CreateDefaultSubobject<UMFG_EffectsComponent>(TEXT("EffectsComponent"));

	MaxUltimateXP = 100.0f;
	MaxUltimateDuration = 8.0f;
	UltimateFrequency = 0.5f;
	bUltimateWithTick = true;

	UltimatePlayRate = 2.0f;
	PlayRate = 1.0f;
	UltimateShotFrequency = 0.25;
	UltimateWalkSpeed = RunSpeed;
	UltimateRunSpeed = 2200.0f;
	UltimateWeaponDamageMultiplier = 2.0f;

	bCanUseAbility = true;
	bIsUsingAbility = false;
	LaserShotsLeft = 3.0f;
	LaserTraceLenght = 8000.0f;
	AbilitySocketName = "Muzzle_05";
	LaserReloadTimeSpeed = 3.0f;
	MaximumLaserShots = 3.0f;

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
	InitializeReferences();
	CreateInitialWeapon();
	MeleeDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &AMFG_Character::MakeMeleeDamage);

	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &AMFG_Character::OnHealthChange);

	EffectsComponent->OnBurningStateChangeDelegate.AddDynamic(this, &AMFG_Character::OnBurningStateChange);
}

void AMFG_Character::InitializeReferences()
{
	if (IsValid(GetMesh()))
	{
		MyAnimInstance = GetMesh()->GetAnimInstance();
	}

	GameModeReference = Cast<AMFG_GameMode>(GetWorld()->GetAuthGameMode());
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
		StopRunning();
	}

	if (bIsCrouching)
	{
		Super::Crouch();
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

void AMFG_Character::StopRunning()
{
	bIsRunning = false;

	SetCharacterSpeed();
}

void AMFG_Character::SetCharacterSpeed()
{
	if (bIsRunning)
	{
		Speed = bIsUsingUltimate ? UltimateRunSpeed : RunSpeed;
		GetCharacterMovement()->MaxWalkSpeed = Speed;
	}
	else
	{
		Speed = bIsUsingUltimate ? UltimateWalkSpeed : WalkSpeed;
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
			WeaponInitialDamage = CurrentWeapon->GetCurrentDamage();
			CurrentWeapon->SetCharacterOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}
}

void AMFG_Character::StartWeaponAction()
{
	if (!bCanUseWeapon)
	{
		return;
	}

	bIsShooting = true;
	if (bIsRunning)
	{
		StopRunning();
	}

	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->SetCurrentDamage(bIsUsingUltimate ? WeaponInitialDamage * UltimateWeaponDamageMultiplier : WeaponInitialDamage);

		CurrentWeapon->StartAction();

		if (bIsUsingUltimate)
		{
			//CurrentWeapon->SetCurrentDamage(WeaponInitialDamage * UltimateWeaponDamageMultiplier);

			AMFG_Rifle* RifleEquipped = Cast<AMFG_Rifle>(CurrentWeapon);
			if (IsValid(RifleEquipped))
			{
				GetWorld()->GetTimerManager().SetTimer(TimerHandle_AutomaticShot, CurrentWeapon, &AMFG_Weapon::StartAction, UltimateShotFrequency, true);
			}
		}
	}
}

void AMFG_Character::StopWeaponAction()
{
	if (!bCanUseWeapon)
	{
		return;
	}

	bIsShooting = false;
	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->StopAction();

		if (bIsUsingUltimate)
		{
			CurrentWeapon->SetCurrentDamage(WeaponInitialDamage);

			AMFG_Rifle* RifleEquipped = Cast<AMFG_Rifle>(CurrentWeapon);
			if (IsValid(RifleEquipped))
			{
				GetWorld()->GetTimerManager().ClearTimer(TimerHandle_AutomaticShot);
			}
		}
	}
}

void AMFG_Character::SetWeaponBehavior()
{
	if (!bIsUsingUltimate)
	{
		StopWeaponAction();
		AMFG_Rifle* RifleEquipped = Cast<AMFG_Rifle>(CurrentWeapon);
		if (IsValid(RifleEquipped))
		{
			RifleEquipped->bIsAutomatic = !RifleEquipped->bIsAutomatic;
			//bIsWeaponAutomatic = !bIsWeaponAutomatic;
		}
	}
}

void AMFG_Character::StartMelee()
{
	if (bIsRunning)
	{
		StopRunning();
	}

	if (bIsDoingMelee && !bCanMakeCombos)
	{
		return;
	}

	if (bCanMakeCombos)
	{
		if (bIsDoingMelee)
		{
			if (bIsComboEnable)
			{
				if (CurrentComboMultiplier < MaxComboMultiplier)
				{
					CurrentComboMultiplier++;
					SetComboEnable(false);
				}
				else
				{

				}
			}
			else
			{
				return;
			}
		}
	}

	if (IsValid(InteractiveObject))
	{
		InteractiveObject->HitObject();
	}

	if (IsValid(MyAnimInstance) && IsValid(MeleeMontage))
	{
		MyAnimInstance->Montage_Play(MeleeMontage, bIsUsingUltimate ? UltimatePlayRate : MeleePlayRate);
	}

	SetMeleeState(true);
}

void AMFG_Character::StopMelee()
{

}

void AMFG_Character::StartAbility()
{
	if (LaserShotsLeft > 0 && bCanUseAbility && !bIsUsingAbility)
	{
		SetAbilityState(true);
		if (IsValid(MyAnimInstance) && IsValid(AbilityMontage))
		{
			MyAnimInstance->Montage_Play(AbilityMontage);
		}

		LaserShotsLeft--;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_ReloadAbilityShots, this, &AMFG_Character::AbilityReload, LaserReloadTimeSpeed, true);

		BP_StartAbility();
	}
}

void AMFG_Character::StopAbility()
{

}

void AMFG_Character::StartUltimate()
{
	if (bCanUseUltimate && !bIsUsingUltimate)
	{
		CurrentUltimateDuration = MaxUltimateDuration;

		bCanUseUltimate = false;

		if (IsValid(MyAnimInstance) && IsValid(UltimateMontage))
		{
			GetCharacterMovement()->MaxWalkSpeed = 0.0f;
			bCanUseWeapon = false;

			if (IsValid(UltimateWeaponEffect))
			{
				UltimateWeaponEffectComponent = UGameplayStatics::SpawnEmitterAttached(UltimateWeaponEffect, GetMesh(), MeleeSocketName);
			}

			const float StartUltimateMontageDuration = MyAnimInstance->Montage_Play(UltimateMontage);
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_BeginUltimateBehavior, this, &AMFG_Character::BeginUltimateBehavior, StartUltimateMontageDuration, false);
		}
		else
		{
			BeginUltimateBehavior();
		}

		BP_StartUltimate();
	}
}

void AMFG_Character::StopUltimate()
{

}

void AMFG_Character::SetAbilityBehavior()
{
	AActor* CurrentCharacterActor = GetOwner();

	if (IsValid(CurrentCharacterActor))
	{
		FVector EyeLocation;
		FRotator EyeRotation;

		CurrentCharacterActor->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + (ShotDirection * LaserTraceLenght);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;

		FVector TraceEndPoint = TraceEnd;

		FHitResult HitResult;
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEnd, COLLISION_WEAPON, QueryParams);

		if (bHit)
		{
			TraceEndPoint = HitResult.ImpactPoint;

			if (IsValid(AbilityEffect))
			{
				FVector LaserSocketLocation = GetMesh()->GetSocketLocation(AbilitySocketName);
				FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(LaserSocketLocation, TraceEndPoint);

				AMFG_LaserProjectile* CurrentProjectile = GetWorld()->SpawnActor<AMFG_LaserProjectile>(LaserProjectileClass, LaserSocketLocation, NewRotation);
				if (bIsUsingUltimate)
				{
					CurrentProjectile->SetNewDamageValue(CurrentProjectile->GetLaserProjectileDamage() * UltimateWeaponDamageMultiplier);
				}
			}
		}
	}
}

void AMFG_Character::AbilityReload()
{
	if (LaserShotsLeft < MaximumLaserShots)
	{
		LaserShotsLeft++;
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_ReloadAbilityShots);
	}

	BP_AbilityReload();
}

void AMFG_Character::MakeMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{
		if (bIsUsingUltimate)
		{
			UGameplayStatics::ApplyPointDamage(OtherActor, (MeleeDamage * CurrentComboMultiplier) * UltimateWeaponDamageMultiplier, SweepResult.Location, SweepResult, GetInstigatorController(), this, nullptr);
		}
		else 
		{
			UGameplayStatics::ApplyPointDamage(OtherActor, MeleeDamage * CurrentComboMultiplier, SweepResult.Location, SweepResult, GetInstigatorController(), this, nullptr);
		}
	}
}

void AMFG_Character::OnHealthChange(UMFG_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (HealthComponent->IsDead())
	{
		if (IsValid(GameModeReference))
		{
			GameModeReference->GameOver(this);
		}
	}
}

void AMFG_Character::OnBurningStateChange(UMFG_EffectsComponent* CurrentEffectsComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (EffectsComponent->IsBurning())
	{
		BurningEffectComponent->SetVisibility(true);
		/*if (IsValid(BurningEffect))
		{
			BurningEffectComponent = UGameplayStatics::SpawnEmitterAttached(BurningEffect, GetMesh(), EffectsSocketName);
		}*/
	}
	else
	{
		if (IsValid(BurningEffectComponent))
		{
			BurningEffectComponent->SetVisibility(false);
		}
		/*if (IsValid(BurningEffectComponent))
		{
			BurningEffectComponent->DestroyComponent();
		}*/
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

	if (bUltimateWithTick && bIsUsingUltimate)
	{
		UpdateUltimateDuration(DeltaTime);
	}

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

	PlayerInputComponent->BindAction("Melee", IE_Pressed, this, &AMFG_Character::StartMelee);
	PlayerInputComponent->BindAction("Melee", IE_Released, this, &AMFG_Character::StopMelee);

	PlayerInputComponent->BindAction("InitialAbility", IE_Pressed, this, &AMFG_Character::StartAbility);
	PlayerInputComponent->BindAction("InitialAbility", IE_Released, this, &AMFG_Character::StopAbility);

	PlayerInputComponent->BindAction("Ultimate", IE_Pressed, this, &AMFG_Character::StartUltimate);
	PlayerInputComponent->BindAction("Ultimate", IE_Released, this, &AMFG_Character::StopUltimate);

}

void AMFG_Character::AddKey(FName NewKey)
{
	DoorKeys.Add(NewKey);
}

bool AMFG_Character::HasKey(FName KeyTag)
{
	return DoorKeys.Contains(KeyTag);
}

void AMFG_Character::SetMeleeDetectorCollision(ECollisionEnabled::Type NewCollisionState)
{
	MeleeDetectorComponent->SetCollisionEnabled(NewCollisionState);
}

void AMFG_Character::SetMeleeState(bool NewState)
{
	bIsDoingMelee = NewState;
	bCanUseWeapon = !NewState;
}

void AMFG_Character::SetAbilityState(bool NewState)
{
	bIsUsingAbility = NewState;
	bCanUseAbility = !NewState;
}

void AMFG_Character::SetComboEnable(bool NewState)
{
	bIsComboEnable = NewState;
}

void AMFG_Character::ResetCombo()
{
	SetComboEnable(false);
	CurrentComboMultiplier = 1.0f;
}

void AMFG_Character::GainUltimateXP(float XPGained)
{
	if (bCanUseUltimate || bIsUsingUltimate)
	{
		return;
	}

	CurrentUltimateXP = FMath::Clamp(CurrentUltimateXP + XPGained, 0.0f, MaxUltimateXP);

	if (CurrentUltimateXP == MaxUltimateXP)
	{
		bCanUseUltimate = true;
	}

	BP_GainUltimateXP(XPGained);
}

void AMFG_Character::UpdateUltimateDuration(float Value)
{
	CurrentUltimateDuration = FMath::Clamp(CurrentUltimateDuration - Value, 0.0f, MaxUltimateDuration);

	BP_UpdateUltimateDuration(Value);

	if (CurrentUltimateDuration == 0.0f)
	{
		CurrentUltimateXP = 0.0f;
		bIsUsingUltimate = false;

		SetCharacterSpeed();
		PlayRate = 1.0f;
		CurrentWeapon->SetCurrentDamage(WeaponInitialDamage);

		if (IsValid(UltimateWeaponEffectComponent))
		{
			UltimateWeaponEffectComponent->DestroyComponent();
		}

		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_AutomaticShot);

		if (!bUltimateWithTick)
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Ultimate);
		}

		BP_StopUltimate();
	}
}

void AMFG_Character::UpdateUltimateDurationWithTimer()
{
	UpdateUltimateDuration(UltimateFrequency);
}

void AMFG_Character::BeginUltimateBehavior()
{
	bCanUseWeapon = true;
	bIsUsingUltimate = true;
	SetCharacterSpeed();
	PlayRate = UltimatePlayRate;

	if (!bUltimateWithTick)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Ultimate, this, &AMFG_Character::UpdateUltimateDurationWithTimer, UltimateFrequency, true);
	}
}
