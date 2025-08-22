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

	FGameplayEffectSpecHandle TargetEffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, TargetEffectContext);
	const FGameplayEffectSpec* TargetEffectSpec = TargetEffectSpecHandle.Data.Get();
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*TargetEffectSpec);

	const bool bIsInfinite = TargetEffectSpec->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	const bool bShouldStore = InfiniteERPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap;
	if (bIsInfinite && bShouldStore)
	{
		ActiveEffectHandles.Add(ActiveGameplayEffectHandle, TargetASC);
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEAPolicy == EEffectApplicationPolicy::ApplyOnOverlap && InstantGameplayEffectClass)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEAPolicy == EEffectApplicationPolicy::ApplyOnOverlap && DurationGameplayEffectClass)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEAPolicy == EEffectApplicationPolicy::ApplyOnOverlap && InfiniteGameplayEffectClass)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEAPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap && InstantGameplayEffectClass)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEAPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap && DurationGameplayEffectClass)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEAPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap && InfiniteGameplayEffectClass)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	// Remove infinite effect
	if (InfiniteERPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap && InfiniteGameplayEffectClass)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;

		// Search effect, remove, add it to remove list
		TArray<FActiveGameplayEffectHandle> RemovedEffects; 
		for (auto HandlePair : ActiveEffectHandles)
		{
			if (TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				RemovedEffects.Add(HandlePair.Key);
			}
		}

		// Remove it from list of effects
		for (auto& Handle : RemovedEffects)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}
