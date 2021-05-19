// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MFG_HUDMessage.generated.h"

class UWidgetAnimation;
/**
 * 
 */
UCLASS()
class PROJECT_MFG_API UMFG_HUDMessage : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
		float HideMessageTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setup")
		FString WidgetMessage;

	UPROPERTY(meta = (BindWidgetAnim))
		UWidgetAnimation* HideMessageAnim;

	FTimerHandle TimerHandle_HideMessage;

public:

	UFUNCTION(BlueprintCallable)
	void InitializeWidget();

	UFUNCTION(BlueprintCallable)
	void ShowWidgetMessage();

	UFUNCTION(BlueprintCallable)
	void HideWidgetMessage();

	UFUNCTION(BlueprintCallable)
	void SetWidgetMessage(FString NewMessage) { WidgetMessage = NewMessage; };
	
};
