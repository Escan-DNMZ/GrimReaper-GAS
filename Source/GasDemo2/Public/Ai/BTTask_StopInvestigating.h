// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_StopInvestigating.generated.h"

/**
 * 
 */
UCLASS()
class GASDEMO2_API UBTTask_StopInvestigating : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_StopInvestigating();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;



	
};
