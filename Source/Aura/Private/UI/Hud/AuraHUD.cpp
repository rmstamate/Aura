// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Hud/AuraHUD.h"

#include "Blueprint/UserWidget.h"

void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	Widget->AddToViewport();
}
