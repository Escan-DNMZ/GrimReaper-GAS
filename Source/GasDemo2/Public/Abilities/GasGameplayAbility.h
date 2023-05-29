// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GasDemo2/GasDemo2.h"
#include "GasGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class GASDEMO2_API UGasGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGasGameplayAbility();

	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
	EGasAbilityInputId AbilityInputID = EGasAbilityInputId::None;
};
