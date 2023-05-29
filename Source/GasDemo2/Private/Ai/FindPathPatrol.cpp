// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/FindPathPatrol.h"
#include <Ai/SeverogAIController.h>
#include "BehaviorTree/BlackboardComponent.h"
#include "Ai/BlackboardKeys.h"
#include <Actors/Enemy/WalkingDeads.h>
#include <Kismet/GameplayStatics.h>
#include <GasDemo2/GasDemo2Character.h>
#include <Ai/Path/PathActor.h>

UFindPathPatrol::UFindPathPatrol(FObjectInitializer const& object_initializer)
{

}

EBTNodeResult::Type UFindPathPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto cont = Cast<ASeverogAIController>(OwnerComp.GetAIOwner());

	const int index = cont->get_blackboard()->GetValueAsInt(bb_keys::PatrolPathIndex);
	
	AGasDemo2Character* Enemy = Cast<AGasDemo2Character>(cont->GetPawn());
	const FVector PatrolPath = Enemy->get_patrol_path()->get_patrol_Index(index);

	FVector const global_point = Enemy->get_patrol_path()->GetActorTransform().TransformPosition(PatrolPath);

	cont->get_blackboard()->SetValueAsVector(bb_keys::PatrolPathVector,global_point);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
