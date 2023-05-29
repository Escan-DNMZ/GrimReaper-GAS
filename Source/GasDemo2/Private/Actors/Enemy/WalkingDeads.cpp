// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Enemy/WalkingDeads.h"
#include <AbilitySystemComponent.h>
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AttributeSet/GasAttributeSet.h"

AWalkingDeads::AWalkingDeads()
{
	
}

void AWalkingDeads::Tick(float DeltaTime)
{

	if (GetCharacterMovement() && AttributeSet)
	{
		float MovementSpeed = AttributeSet->GetMovementSpeed();
		GetCharacterMovement()->SetDefaultMovementMode();
		GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;

	}
}

void AWalkingDeads::StartSprint()
{
	const FGameplayTag Tag = FGameplayTag::RequestGameplayTag("Buff.Sprint");
	FGameplayEventData Payload = FGameplayEventData();
	Payload.Instigator = GetInstigator();
	Payload.Target = this;
	Payload.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(this);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetInstigator(), Tag, Payload);

	
}

void AWalkingDeads::Stun()
{
	const FGameplayTag Tag = FGameplayTag::RequestGameplayTag("Stun.Stunned");
	FGameplayEventData Payload = FGameplayEventData();
	Payload.Instigator = GetInstigator();
	Payload.Target = this;
	Payload.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(this);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetInstigator(), Tag, Payload);
}

