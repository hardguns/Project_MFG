// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Character/MFG_ANSAbility.h"
#include "MFG_Character.h"

void UMFG_ANSAbility::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if (IsValid(CharacterActor))
	{
		AMFG_Character* Character = Cast<AMFG_Character>(CharacterActor);
		if (IsValid(Character))
		{
			Character->SetAbilityState(true);
		}
	}
}

void UMFG_ANSAbility::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if (IsValid(CharacterActor))
	{
		AMFG_Character* Character = Cast<AMFG_Character>(CharacterActor);
		if (IsValid(Character))
		{
			Character->SetAbilityState(false);
		}
	}
}
