// Fill out your copyright notice in the Description page of Project Settings.

#include "Ai/BTTask_TargetLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "Ai/SeverogAIController.h"
//#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Ai/BlackboardKeys.h"


UBTTask_TargetLocation::UBTTask_TargetLocation(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Find Random Location");
}

EBTNodeResult::Type UBTTask_TargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const cont = Cast<ASeverogAIController>(OwnerComp.GetAIOwner());
	auto const GrimReaper = cont->GetPawn();

	FVector const origin = GrimReaper->GetActorLocation();
	FNavLocation Loc;

	UNavigationSystemV1* const nav_sys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (nav_sys->GetRandomPointInNavigableRadius(origin, search_readius, Loc,nullptr)) {
		cont->get_blackboard()->SetValueAsVector(bb_keys::Target_Location,Loc.Location);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
