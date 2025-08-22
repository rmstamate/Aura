// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "GameplayAbilities/Public/GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(50.f);
	InitMaxHealth(100.f);
	InitMana(50.f);
	InitMaxMana(100.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties TargetProps;
	FEffectProperties SourceProps;
	SetEffectProperties(Data, SourceProps, TargetProps);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("%f value"), GetHealth());
	}
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& SourceProps, FEffectProperties& TargetProps)
{
	// Source = causer
	// Target = target but owner of this
	SourceProps.EffectContextHandle = Data.EffectSpec.GetContext();
	TargetProps.EffectContextHandle = Data.EffectSpec.GetContext();

	SourceProps.AbilitySystemComponent = SourceProps.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	if (IsValid(SourceProps.AbilitySystemComponent) && SourceProps.AbilitySystemComponent->AbilityActorInfo.IsValid())
	{
		SourceProps.AvatarActor = SourceProps.AbilitySystemComponent->AbilityActorInfo->AvatarActor.Get();
		SourceProps.AvatarController = SourceProps.AbilitySystemComponent->AbilityActorInfo->PlayerController.Get();

		if (SourceProps.AvatarController == nullptr && SourceProps.AvatarActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(SourceProps.AvatarActor))
			{
				SourceProps.AvatarController = Pawn->GetController();
			}
		}

		if (SourceProps.AvatarController)
		{
			SourceProps.ActorCharacter = Cast<ACharacter>(SourceProps.AvatarController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid())
	{
		TargetProps.AvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetProps.AvatarController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetProps.ActorCharacter = Cast<ACharacter>(TargetProps.AvatarActor);
		TargetProps.AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetProps.AvatarActor);
	}
}
