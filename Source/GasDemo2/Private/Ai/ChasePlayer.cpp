// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/ChasePlayer.h"
#include "Ai/SeverogAIController.h"
#include <BehaviorTree/BlackboardComponent.h>
#include <Ai/BlackboardKeys.h>
#include <Blueprint/AIBlueprintHelperLibrary.h>

UChasePlayer::UChasePlayer(const FObjectInitializer& object_initializer)
{
	NodeName = "Chase Player";
}

EBTNodeResult::Type UChasePlayer::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8 nore_memory)
{
	ASeverogAIController* AiController = Cast<ASeverogAIController>(owner_comp.GetAIOwner());
	FVector const player_location = AiController->get_blackboard()->GetValueAsVector(bb_keys::Target_Location);

	AiController->MoveToLocation(player_location);

	
	return EBTNodeResult::Succeeded;
}
