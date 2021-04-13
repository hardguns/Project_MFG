// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/Character/MFG_ANEndJumpSound.h"
#include "Sound/SoundCue.h"
#include "MFG_Character.h"

void UMFG_ANEndJumpSound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* CharacterActor = MeshComp->GetOwner();
	if (IsValid(CharacterActor)) 
	{
		AMFG_Character* Character = Cast<AMFG_Character>(CharacterActor);
		if (IsValid(Character))
		{
			USoundCue* EndJumpSound = Character->GetEndJumpSound();
			Character->PlayStepSound(EndJumpSound);
		}
	}
}
