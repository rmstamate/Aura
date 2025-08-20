// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/AuraUserWidget.h"

void UAuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	checkf(InWidgetController, TEXT("The reference is broken?"));
	if (InWidgetController)
	{
		WidgetController = InWidgetController;
		// WidgetControllerSet();
	}
}
