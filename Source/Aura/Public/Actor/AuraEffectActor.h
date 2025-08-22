// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply,
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove,
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	// Data
	UPROPERTY()
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles; 

	// Settings
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Effects")
	bool bDestroyOnEffectRemoval = false;

	// Effect apply policy
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Effects")
	EEffectApplicationPolicy InstantEAPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Effects")
	EEffectApplicationPolicy DurationEAPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Effects")
	EEffectApplicationPolicy InfiniteEAPolicy = EEffectApplicationPolicy::DoNotApply;

	// Effect remove policy
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Effects")
	EEffectRemovalPolicy InfiniteERPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;

	// Functions
	UFUNCTION(BlueprintCallable, Category="Effects")
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable, Category="Effects")
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category="Effects")
	void OnEndOverlap(AActor* TargetActor);
};
