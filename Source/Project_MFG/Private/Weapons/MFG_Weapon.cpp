// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/MFG_Weapon.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
AMFG_Weapon::AMFG_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Damage = 20.0f;
	WeaponPitchRecoil = 0;
	WeaponYawRecoil = 0;
	CameraPitchRecoilShake = 0;
	CameraYawRecoilShake = 0;
}

FRecoilBehaviourStruct::FRecoilBehaviourStruct()
{
	MinPitchRecoil = 0;
	MaxPitchRecoil = 0;
	MinYawRecoil = 0;
	MaxYawRecoil = 0;
	MinCameraPitchShake = 0;
	MaxCameraPitchShake = 0;
	MinCameraYawShake = 0;
	MaxCameraYawShake = 0;
}

// Called when the game starts or when spawned
void AMFG_Weapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMFG_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMFG_Weapon::StartAction()
{
	BP_StartAction();
	PlaySound(ShotSound);
}

void AMFG_Weapon::StopAction()
{
	BP_StopAction();
}

void AMFG_Weapon::SetCharacterOwner(ACharacter* NewOwner)
{
	if (IsValid(NewOwner))
	{
		SetOwner(NewOwner);
		CurrentOwnerCharacter = NewOwner;
	}
}

void AMFG_Weapon::SetCurrentDamage(float NewDamage)
{
	Damage = NewDamage;
}

void AMFG_Weapon::GetRecoilInfo(FName RowName)
{
	EnemyBehaviorRow = new FRecoilBehaviourStruct();
	CharacterBehaviorRow = new FRecoilBehaviourStruct();

	if (IsValid(EnemyRecoilBehaviourDT))
	{
		EnemyRecoilBehaviourDT->RowStruct = FRecoilBehaviourStruct::StaticStruct();
		EnemyBehaviorRow = EnemyRecoilBehaviourDT->FindRow<FRecoilBehaviourStruct>(RowName, "");
	}

	if (IsValid(CharacterRecoilBehaviourDT))
	{
		CharacterRecoilBehaviourDT->RowStruct = FRecoilBehaviourStruct::StaticStruct();
		CharacterBehaviorRow = CharacterRecoilBehaviourDT->FindRow<FRecoilBehaviourStruct>(RowName, "");
	}
}

void AMFG_Weapon::PlaySound(USoundCue* SoundCue, bool bIs3D, FVector SoundLocation)
{
	if (!IsValid(SoundCue))
	{
		return;
	}

	if (bIs3D)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundCue, SoundLocation);
	}
	else
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundCue);
	}
}

