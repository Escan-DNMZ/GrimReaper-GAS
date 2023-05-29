// Fill out your copyright notice in the Description page of Project Settings.

#include "Ai/BTTask_AttackNode.h"
#include <GasDemo2/GasDemo2Character.h>
#include <AbilitySystemComponent.h>
#include <Abilities/GasGameplayAbility.h>
#include "AIController.h"
#include <AbilitySystemBlueprintLibrary.h>
#include <Actors/Enemy/WalkingDeads.h>
#include <Kismet/GameplayStatics.h>
#include "Kismet/KismetMathLibrary.h"

UBTTask_AttackNode::UBTTask_AttackNode() {
	
}


EBTNodeResult::Type UBTTask_AttackNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	ACharacter* ControlledCharacter = AIController->GetCharacter();
	if (!ControlledCharacter) return EBTNodeResult::Failed;

	ACharacter* ControllerPawn = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	AGasDemo2Character* Character = Cast<AGasDemo2Character>(ControllerPawn);
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(player);
	if (!AbilitySystemComponent) return EBTNodeResult::Failed;

	
	if (Character)
	{
		if (UKismetMathLibrary::Vector_Distance(ControlledCharacter->GetActorLocation(), player->GetActorLocation()) < 400)
		{
			FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();

			FGameplayEffectSpecHandle EffectSpec = AbilitySystemComponent->MakeOutgoingSpec(Effect, 0, EffectContext);
			if (EffectSpec.IsValid())
			{
				AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*EffectSpec.Data.Get(), AbilitySystemComponent);
			}

			FGameplayEffectSpecHandle EffectSpec2 = AbilitySystemComponent->MakeOutgoingSpec(StunEffect, 0, EffectContext);
			if (EffectSpec.IsValid())
			{
				AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*EffectSpec2.Data.Get(), AbilitySystemComponent);
			}

			if (bDoOnce)
			{
				FGameplayEffectSpecHandle EffectSpec3 = AbilitySystemComponent->MakeOutgoingSpec(SprintEffect, 0, EffectContext);
				if (EffectSpec.IsValid())
				{
					AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*EffectSpec3.Data.Get(), AbilitySystemComponent);
				}
				bDoOnce = false;
			}
			/*	AbilitySystemComponent->TryActivateAbilityByClass(Ability, true);*/
			UE_LOG(LogTemp, Warning, TEXT("cast!"));
			return EBTNodeResult::Succeeded;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Can't cast!"));
	return EBTNodeResult::Failed;
	
}
