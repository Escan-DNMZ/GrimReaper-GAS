// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeSet/GasAttributeSet.h"

#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "GasDemo2/GasDemo2Character.h"



void UGasAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGasAttributeSet, Mana, OldMana);
}

void UGasAttributeSet::OnRep_StunCount(const FGameplayAttributeData& OldStun)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGasAttributeSet, StunCount, OldStun);
}

void UGasAttributeSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGasAttributeSet, MovementSpeed, OldSpeed);
}

void UGasAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGasAttributeSet, Health, OldHealth);
}
void UGasAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGasAttributeSet, Health);
	DOREPLIFETIME(UGasAttributeSet, Mana);
	DOREPLIFETIME(UGasAttributeSet, StunCount);
	DOREPLIFETIME(UGasAttributeSet, MovementSpeed);

}