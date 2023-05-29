// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/IncrementPathIndex.h"
#include <Ai/SeverogAIController.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <GasDemo2/GasDemo2Character.h>
#include <Ai/Path/PathActor.h>
#include <Ai/BlackboardKeys.h>
#include "cmath"

EBTNodeResult::Type UIncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ASeverogAIController* const controller = Cast<ASeverogAIController>(OwnerComp.GetAIOwner());
	AGasDemo2Character* const Grim = Cast<AGasDemo2Character>(controller->GetPawn());

	int const no_of_points = Grim->get_patrol_path()->num();
	int const min_index = 0;
	int const max_index = no_of_points - 1;

	int index = controller->get_blackboard()->GetValueAsInt(bb_keys::PatrolPathIndex);
	if (bIsDirectional)
	{
		if (index >= max_index && direction == EDirectionType::Forward)
		{
			direction = EDirectionType::Reverse;
		}
		else if (index == min_index && direction == EDirectionType::Reverse)
		{
			direction = EDirectionType::Forward;
		}

	}
	
	controller->get_blackboard()->SetValueAsInt(bb_keys::PatrolPathIndex, (direction == EDirectionType::Forward ? std::abs(++index) : std::abs(--index)) % no_of_points);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
