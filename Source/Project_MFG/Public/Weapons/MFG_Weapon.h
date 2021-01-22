// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MFG_Weapon.generated.h"

class UDamageType;
class ACharacter;
class UDataTable;

USTRUCT(BlueprintType)
struct FRecoilBehaviourStruct
{
	GENERATED_USTRUCT_BODY()

		FRecoilBehaviourStruct();

public:

	UPROPERTY(EditAnywhere)
		float MinPitchRecoil;

	UPROPERTY(EditAnywhere)
		float MaxPitchRecoil;

	UPROPERTY(EditAnywhere)
		float MinYawRecoil;

	UPROPERTY(EditAnywhere)
		float MaxYawRecoil;

	UPROPERTY(EditAnywhere)
		float MinCameraPitchShake;

	UPROPERTY(EditAnywhere)
		float MaxCameraPitchShake;

	UPROPERTY(EditAnywhere)
		float MinCameraYawShake;

	UPROPERTY(EditAnywhere)
		float MaxCameraYawShake;

};

UCLASS()
class PROJECT_MFG_API AMFG_Weapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMFG_Weapon();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float WeaponPitchRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float WeaponYawRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float CameraPitchRecoilShake;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
		float CameraYawRecoilShake;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recoil")
		UDataTable* EnemyRecoilBehaviourDT;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recoil")
		UDataTable* CharacterRecoilBehaviourDT;

	ACharacter* CurrentOwnerCharacter;

	FRecoilBehaviourStruct* EnemyBehaviorRow;

	FRecoilBehaviourStruct* CharacterBehaviorRow;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Weapon")
	void BP_StartAction();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Weapon")
	void BP_StopAction();

public:	
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void StartAction();

	UFUNCTION(BlueprintCallable)
	virtual	void StopAction();

	UFUNCTION(BlueprintCallable)
	void SetCharacterOwner(ACharacter* NewOwner);

	void SetCurrentDamage(float NewDamage);

	float GetCurrentDamage() { return Damage; };

	void GetRecoilInfo(FName RowName);

};
