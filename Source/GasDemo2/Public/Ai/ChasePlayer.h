// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "ChasePlayer.generated.h"

/**
 * 
 */
UCLASS()
class GASDEMO2_API UChasePlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UChasePlayer(const FObjectInitializer& object_initializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8 nore_memory);
	
};
