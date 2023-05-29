// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "FindPathPatrol.generated.h"

/**
 * 
 */
UCLASS()
class GASDEMO2_API UFindPathPatrol : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
		UFindPathPatrol(FObjectInitializer const& object_initializer);
		EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
