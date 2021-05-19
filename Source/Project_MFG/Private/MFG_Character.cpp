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
#include "Objects/MFG_Shield.h"
#include "Core/MFG_GameInstance.h" 
#include "Abilities/MFG_Ability.h"
#include "UI/HUD/MFG_HUD.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

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

	StepSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("StepSoundComponent"));
	StepSoundComponent->SetupAttachment(RootComponent);

	VoiceSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("VoiceSoundComponent"));
	VoiceSoundComponent->SetupAttachment(RootComponent);

	AbilitiesSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AbilitiesSoundComponent"));
	AbilitiesSoundComponent->SetupAttachment(RootComponent);

	MaxUltimateXP = 100.0f;
	MaxUltimateDuration = 8.0f;
	MaxUltimateReloadSeconds = 240.0f;
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

	InteractiveObject = NULL;

	MainMenuMapName = "MainMenuMap";

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

	//Sets different references from map
	InitializeReferences();
	//Get weapon subclass and initialize weapon
	CreateInitialWeapon();
	//Sets abilities to player or enemy
	CreateInitialAbilities();
	//Starts timer that loads ultimate automatically
	StartUltimateLoading();

	//Delegates subscriptions
	MeleeDetectorComponent->OnComponentBeginOverlap.AddDynamic(this, &AMFG_Character::MakeMeleeDamage);

	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &AMFG_Character::OnHealthChange);
	HealthComponent->OnHealthUpdateDelegate.AddDynamic(this, &AMFG_Character::OnHealthUpdate);

	EffectsComponent->OnBurningStateChangeDelegate.AddDynamic(this, &AMFG_Character::OnBurningStateChange);
}

void AMFG_Character::InitializeReferences()
{
	if (IsValid(GetMesh()))
	{
		MyAnimInstance = GetMesh()->GetAnimInstance();
	}

	GameModeReference = Cast<AMFG_GameMode>(GetWorld()->GetAuthGameMode());

	GameInstanceReference = Cast<UMFG_GameInstance>(GetWorld()->GetGameInstance());
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
		//Sets crouch behavior from ACharacter class
		Super::Crouch();
	}
	else
	{
		//Sets uncrouch behavior from ACharacter class
		Super::UnCrouch();
		//Sets speed depending on player action (running, walking, crouching)
		SetCharacterSpeed();
	}
}

void AMFG_Character::RollStart()
{
	bIsRolling = true;
}

void AMFG_Character::Run()
{
	bIsRunning = !bIsRunning;

	if (bIsCrouching)
	{
		//Sets uncrouch behavior from ACharacter class
		Super::UnCrouch();
		bIsCrouching = false;
	}

	if (bIsShooting)
	{
		//Player is not able to action weapon when running
		StopWeaponAction();
	}

	SetCharacterSpeed();
}

void AMFG_Character::StopRunning()
{
	bIsRunning = false;

	SetCharacterSpeed();
}

//Set speed depending on player action (running, walking, crouching)
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

//Interact with object to activate it when player press a key
void AMFG_Character::DoAction()
{
	if (IsValid(InteractiveObject))
	{
		AMFG_Activable* ActivableObj = Cast<AMFG_Activable>(InteractiveObject);
		if (IsValid(ActivableObj))
		{
			ActivableObj->CheckActivable(this);
		}
	}
}

//Gets player current position depending on which camera is used for the game
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
	//Sets jump behavior from ACharacter class
	Super::Jump();

	if (!bIsCrouching)
	{
		PlayVoiceSound(BeginJumpSound);
	}
}

void AMFG_Character::StopJumping()
{
	Super::StopJumping();
}

void AMFG_Character::CreateInitialWeapon()
{
	//Uses weapon class to spawn and attach actor in player
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

void AMFG_Character::CreateInitialAbilities()
{
	//Spawns abilities and set it in CharacterAbilities for player's use
	for (TSubclassOf<AMFG_Ability> AbilityClass : CharacterAbilitiesClasses)
	{
		if (IsValid(AbilityClass))
		{
			AMFG_Ability* CurrentAbility = GetWorld()->SpawnActor<AMFG_Ability>(AbilityClass, GetActorLocation(), GetActorRotation());
			if (IsValid(CurrentAbility))
			{
				CharacterAbilities.Add(CurrentAbility);
			}
		}
	}
}

//Fills ultimate automatically
void AMFG_Character::StartUltimateLoading()
{
	//Calculates XPAmount in order to fill bar automatically in a certain time set in MaxUltimateReloadSeconds
	float XPAmount = MaxUltimateXP / MaxUltimateReloadSeconds;

	//Sets timer delegate to bind GainUltimateXP with XPAmount parameter
	TimerDelegate.BindUFunction(this, FName("GainUltimateXP"), XPAmount);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_GainXP, TimerDelegate, 1.0f, true);
}

//Player or enemy attacking
void AMFG_Character::StartWeaponAction()
{
	if (!bCanUseWeapon)
	{
		return;
	}

	bIsShooting = true;
	if (bIsRunning)
	{
		//When using weapon player cannot run 
		StopRunning();
	}

	if (IsValid(CurrentWeapon))
	{
		//Sets weapon damage depending on Ultimate state
		CurrentWeapon->SetCurrentDamage(bIsUsingUltimate ? WeaponInitialDamage * UltimateWeaponDamageMultiplier : WeaponInitialDamage);

		//Triggers weapon action
		CurrentWeapon->StartAction();

		if (bIsUsingUltimate)
		{
			//CurrentWeapon->SetCurrentDamage(WeaponInitialDamage * UltimateWeaponDamageMultiplier);

			//If rifle is equipped when using ultimate shots will be automatic
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

	//Stops weapon action
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

//Sets weapon behavior on player action key
void AMFG_Character::SetWeaponBehavior()
{
	if (!bIsUsingUltimate)
	{
		StopWeaponAction();
		//In case of using rifle rifle set is to automatic
		AMFG_Rifle* RifleEquipped = Cast<AMFG_Rifle>(CurrentWeapon);
		if (IsValid(RifleEquipped))
		{
			RifleEquipped->bIsAutomatic = !RifleEquipped->bIsAutomatic;
		}
	}
}

//Sets melee for player
void AMFG_Character::StartMelee()
{
	if (bIsRunning && GetCharacterType() == EMFG_CharacterType::CharacterType_Player)
	{
		StopRunning();
	}

	if (bIsDoingMelee && !bCanMakeCombos)
	{
		return;
	}

	//Sets (when combos enabled) how damage will be increased on each combo
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

	//If interactive object can be hit action is triggered
	if (IsValid(InteractiveObject))
	{
		InteractiveObject->HitObject();
	}

	//Calculates damage depending on Ultimate State
	if (IsValid(MyAnimInstance) && IsValid(MeleeMontage))
	{
		MyAnimInstance->Montage_Play(MeleeMontage, bIsUsingUltimate ? UltimatePlayRate : MeleePlayRate);
	}

	PlaySound(MeleeSound);

	SetMeleeState(true);
}

void AMFG_Character::StopMelee()
{

}

//Uses ability depending on index (Player action key)
void AMFG_Character::StartAbility(int index)
{
	//Checks if player is using ability to cast new ability
	if (bCanUseAbility && !bIsUsingAbility && CharacterAbilities.IsValidIndex(index))
	{
		if (IsValid(CharacterAbilities[index]))
		{
			CharacterAbilities[index]->CastAbility(this);
		}

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
		if (bIsRunning && GetCharacterType() == EMFG_CharacterType::CharacterType_Player)
		{
			StopRunning();
		}

		CurrentUltimateDuration = MaxUltimateDuration;

		bCanUseUltimate = false;

		PlayVoiceSound(UltimateSound);

		if (IsValid(MyAnimInstance) && IsValid(UltimateMontage))
		{
			//Unable movement to show ultimate animation
			GetCharacterMovement()->MaxWalkSpeed = 0.0f;
			bCanUseWeapon = false;

			if (IsValid(UltimateWeaponEffect))
			{
				UltimateWeaponEffectComponent = UGameplayStatics::SpawnEmitterAttached(UltimateWeaponEffect, GetMesh(), MeleeSocketName);
			}

			//Sets ultimate behavior after montage is played
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

void AMFG_Character::GoToMainMenu()
{
	//Before going to main menu saves data
	if (IsValid(GameInstanceReference))
	{
		GameInstanceReference->SaveData();
	}

	UGameplayStatics::OpenLevel(GetWorld(), MainMenuMapName);
}

void AMFG_Character::PlaySound(USoundCue* PlayableSound)
{
	if (!IsValid(PlayableSound))
	{
		return;
	}

	UGameplayStatics::PlaySound2D(GetWorld(), PlayableSound);
}

void AMFG_Character::SetAbilityBehavior(int index)
{
	//This helps to know at which moment is necessary to do an action in ability montage
	if (CharacterAbilities.IsValidIndex(index))
	{
		if (IsValid(CharacterAbilities[index]))
		{
			CharacterAbilities[index]->SetAbilityBehavior(this);
		}

		BP_StartAbility();
	}
}

void AMFG_Character::MakeMeleeDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (IsValid(OtherActor))
	{
		if (OtherActor == this)
		{
			return;
		}

		AMFG_Character* MeleeTarget = Cast<AMFG_Character>(OtherActor);

		if (IsValid(MeleeTarget))
		{
			//Depending of who's doing the action checks if target is not the same CharacterType
			bool bPlayerAttackingEnemy = GetCharacterType() == EMFG_CharacterType::CharacterType_Player && MeleeTarget->GetCharacterType() == EMFG_CharacterType::CharacterType_Enemy;
			bool bEnemyAttackingPlayer = GetCharacterType() == EMFG_CharacterType::CharacterType_Enemy && MeleeTarget->GetCharacterType() == EMFG_CharacterType::CharacterType_Player;

			if (bPlayerAttackingEnemy || bEnemyAttackingPlayer)
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
		else
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
}

void AMFG_Character::OnHealthChange(UMFG_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!HealthComponent->IsDead())
	{
		PlayVoiceSound(HurtSound);
		bWasDamaged = true;
	}

	if (HealthComponent->IsDead())
	{
		PlayVoiceSound(DeathSound);

		if (GetCharacterType() == EMFG_CharacterType::CharacterType_Player)
		{
			if (IsValid(GameModeReference))
			{
				GameModeReference->GameOver(this);
			}
		}
	}
}

void AMFG_Character::OnHealthUpdate(float Health, float MaxHealth)
{
	if (Health <= 20)
	{
		PlayVoiceSound(AlmostDeadSound);
	}
	else
	{	
		if (VoiceSoundComponent->Sound == AlmostDeadSound)
		{
			//Stop AlmostDeadSound because player or enemy increased their health
			VoiceSoundComponent->Stop();
		}
	}

	if (Health == MaxHealth && bWasDamaged)
	{
		//Play FullHealthSound after healing at maximum
		PlayVoiceSound(FullHealthSound);
		bWasDamaged = false;
	}
}

//Delegate subscribe when player or enemy is being affected by some effect such as Burning
void AMFG_Character::OnBurningStateChange(UMFG_EffectsComponent* CurrentEffectsComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	//When player or enemy is burning
	if (EffectsComponent->IsBurning())
	{
		BurningEffectComponent->SetVisibility(true);
	}
	else
	{
		if (IsValid(BurningEffectComponent))
		{
			BurningEffectComponent->SetVisibility(false);
		}
	}
}

//Controls up and down view for player
void AMFG_Character::AddControllerPitchInput(float value)
{
	//Checks if LookInversion is active
	Super::AddControllerPitchInput(bIsLookInversion ? -value : value);
}

// Called every frame
void AMFG_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If ultimate is used by tick ultimate duration is updated from here
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

	PlayerInputComponent->BindAction<FStartAbilityDelegate>("PrimaryAbility", IE_Pressed, this, &AMFG_Character::StartAbility, 0);

	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &AMFG_Character::DoAction);

	PlayerInputComponent->BindAction("WeaponAction", IE_Pressed, this, &AMFG_Character::StartWeaponAction);
	PlayerInputComponent->BindAction("WeaponAction", IE_Released, this, &AMFG_Character::StopWeaponAction);

	PlayerInputComponent->BindAction("WeaponBehavior", IE_Pressed, this, &AMFG_Character::SetWeaponBehavior);

	PlayerInputComponent->BindAction("Melee", IE_Pressed, this, &AMFG_Character::StartMelee);
	PlayerInputComponent->BindAction("Melee", IE_Released, this, &AMFG_Character::StopMelee);

	PlayerInputComponent->BindAction<FStartAbilityDelegate>("AdditionalAbility", IE_Pressed, this, &AMFG_Character::StartAbility, 1);
	PlayerInputComponent->BindAction("AdditionalAbility", IE_Released, this, &AMFG_Character::StopAbility);

	PlayerInputComponent->BindAction("Ultimate", IE_Pressed, this, &AMFG_Character::StartUltimate);
	PlayerInputComponent->BindAction("Ultimate", IE_Released, this, &AMFG_Character::StopUltimate);

	PlayerInputComponent->BindAction("Exit", IE_Pressed, this, &AMFG_Character::GoToMainMenu);
}

//Adds keys when picked up by player
void AMFG_Character::AddKey(FName NewKey)
{
	DoorKeys.Add(NewKey);
}

//Removes keys when picked up by player
void AMFG_Character::RemoveKey(FName KeyToRemove)
{
	DoorKeys.Remove(KeyToRemove);
}

//Uses Character HealthComponent to add health
bool AMFG_Character::TryAddHealth(float HealthToAdd)
{
	return HealthComponent->TryAddHealth(HealthToAdd);
}

//Check if some key was picked up by player
bool AMFG_Character::HasKey(FName KeyTag)
{
	return DoorKeys.Contains(KeyTag);
}

//Sets if player is using Jetpack ability to be checked in AnimationBlueprint
void AMFG_Character::SetIsUsingBag(bool NewState)
{
	bIsUsingBag = NewState;
}

//Enables or disables melee detector component to make (or not) damage
void AMFG_Character::SetMeleeDetectorCollision(ECollisionEnabled::Type NewCollisionState)
{
	MeleeDetectorComponent->SetCollisionEnabled(NewCollisionState);
}

//Enables or disables weapon or melee usage (depending on action)
void AMFG_Character::SetMeleeState(bool NewState)
{
	bIsDoingMelee = NewState;
	bCanUseWeapon = !NewState;
}

//Enables or disables abilities for player or enemy
void AMFG_Character::SetAbilityState(bool NewState)
{
	bIsUsingAbility = NewState;
	bCanUseAbility = !NewState;
}

//Enables or disables when player can make combos
void AMFG_Character::SetComboEnable(bool NewState)
{
	bIsComboEnable = NewState;
}

void AMFG_Character::ResetCombo()
{
	SetComboEnable(false);
	CurrentComboMultiplier = 1.0f;
}

//Gives XP to player for making some action in game
void AMFG_Character::GainUltimateXP(float XPGained)
{
	if (bCanUseUltimate || bIsUsingUltimate)
	{
		return;
	}

	CurrentUltimateXP = FMath::Clamp(CurrentUltimateXP + XPGained, 0.0f, MaxUltimateXP);
	//Updates ultimate state for widgets and classes
	OnUltimateUpdateDelegate.Broadcast(CurrentUltimateXP, MaxUltimateXP);

	if (CurrentUltimateXP == MaxUltimateXP)
	{
		bCanUseUltimate = true;

		//Stops automatic XP gaining
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_GainXP);

		//Broadcast that ultimate is ready to use
		OnUltimateStatusDelegate.Broadcast(true);
	}

	BP_GainUltimateXP(XPGained);
}

//Changes ultimate duration
void AMFG_Character::UpdateUltimateDuration(float Value)
{
	//Updates ultimate current duration to notify widgets and classes
	CurrentUltimateDuration = FMath::Clamp(CurrentUltimateDuration - Value, 0.0f, MaxUltimateDuration);
	OnUltimateUpdateDelegate.Broadcast(CurrentUltimateDuration, MaxUltimateDuration);

	BP_UpdateUltimateDuration(Value);

	//When duration is 0 resets ultimate behavior to default and starts loading ultimate automatically
	if (CurrentUltimateDuration == 0.0f)
	{
		CurrentUltimateXP = 0.0f;
		bIsUsingUltimate = false;
		OnUltimateStatusDelegate.Broadcast(false);

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

		StartUltimateLoading();

		BP_StopUltimate();
	}
}

void AMFG_Character::UpdateUltimateDurationWithTimer()
{
	UpdateUltimateDuration(UltimateFrequency);
}

//Sets ultimate behavior for player or enemy
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

void AMFG_Character::SetShield(AMFG_Shield* NewShield)
{
	CurrentShield = NewShield;
}

void AMFG_Character::PlayStepSound(USoundCue* StepSound)
{
	StepSoundComponent->SetSound(StepSound);
	StepSoundComponent->Play();
}

//Checks which ability was used to play sound in AbilitiesSoundComponent
void AMFG_Character::PlayAbilitySound(FName AbilityName)
{
	for (AMFG_Ability* Ability : CharacterAbilities)
	{
		if (!IsValid(Ability))
		{
			continue;
		}

		if (Ability->GetAbilityDetails().AbilityName == AbilityName)
		{
			USoundCue* AbilitySound = Ability->GetAbilityDetails().GetAbilitySound();
			if (IsValid(AbilitySound))
			{
				AbilitiesSoundComponent->SetSound(AbilitySound);
				AbilitiesSoundComponent->Play();
			}
		}
	}
}

//Plays everything related with player voice (damage, dialogs, etc.)
void AMFG_Character::PlayVoiceSound(USoundCue* VoiceSound)
{
	if (!IsValid(VoiceSound))
	{
		return;
	}

	VoiceSoundComponent->SetSound(VoiceSound);
	VoiceSoundComponent->Play();
}
