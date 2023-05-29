// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/BTTask_StopInvestigating.h"
#include "Ai/SeverogAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Ai/BlackboardKeys.h"


UBTTask_StopInvestigating::UBTTask_StopInvestigating()
{
	NodeName = TEXT("Stop Investigating");
}

EBTNodeResult::Type UBTTask_StopInvestigating::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto const cont = Cast<ASeverogAIController>(OwnerComp.GetAIOwner());
	cont->get_blackboard()->SetValueAsBool(bb_keys::is_investing,false);



	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
