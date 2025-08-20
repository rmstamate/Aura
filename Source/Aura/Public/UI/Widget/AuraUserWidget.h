// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Controller")
	void SetWidgetController(UObject* InWidgetController);
	
	UPROPERTY(BlueprintReadOnly, Category="Controller")
	TObjectPtr<UObject> WidgetController = nullptr;

// protected:
	// UFUNCTION(BlueprintImplementableEvent, Category="Controller")
	// void WidgetControllerSet();
	
};
