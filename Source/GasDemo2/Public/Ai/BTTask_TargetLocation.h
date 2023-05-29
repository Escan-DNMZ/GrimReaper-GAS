// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "UObject/UObjectGlobals.h"
#include "BTTask_TargetLocation.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GASDEMO2_API UBTTask_TargetLocation : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
	
public:
	UBTTask_TargetLocation(FObjectInitializer const& object_initializer = FObjectInitializer::Get());
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowProtectedAccess = "true"))
		float search_readius = 1500.0f;
};
