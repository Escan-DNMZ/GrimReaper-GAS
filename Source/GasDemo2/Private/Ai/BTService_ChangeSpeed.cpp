// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/BTService_ChangeSpeed.h"
#include <GasDemo2/GasDemo2Character.h>
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_ChangeSpeed::UBTService_ChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}

void UBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	Super::OnBecomeRelevant(owner_comp, node_memory);
	
	auto const cont = owner_comp.GetAIOwner();

	AGasDemo2Character* const Grim = Cast<AGasDemo2Character>(cont->GetPawn());

	Grim->GetCharacterMovement()->MaxWalkSpeed = speed;
}

FString UBTService_ChangeSpeed::GetStaticServiceDescription() const
{
	return FString("Change the GrimReaper speed");
}
