// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;

	checkf(GameplayEffectClass, TEXT("The gameplay effect class is missing"));
	FGameplayEffectContextHandle TargetEffectContext = TargetASC->MakeEffectContext();
	TargetEffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle TargetEffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, TargetEffectContext);
	TargetASC->ApplyGameplayEffectSpecToSelf(*TargetEffectSpecHandle.Data.Get());
}
