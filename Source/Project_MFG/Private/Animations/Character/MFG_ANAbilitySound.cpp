// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Character/MFG_ANAbilitySound.h"
#include "MFG_Character.h"

void UMFG_ANAbilitySound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if (IsValid(CharacterActor))
	{
		AMFG_Character* Character = Cast<AMFG_Character>(CharacterActor);
		if (IsValid(Character))
		{
			Character->PlayAbilitySound(AbilityName);
		}
	}
}
