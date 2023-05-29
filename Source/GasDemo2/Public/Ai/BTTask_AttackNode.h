// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_AttackNode.generated.h"

/**
 * 
 */
UCLASS()
class GASDEMO2_API UBTTask_AttackNode : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

public:
	UBTTask_AttackNode();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	TSubclassOf<class UGameplayEffect> Effect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	TSubclassOf<class UGameplayEffect> StunEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Abilities)
	TSubclassOf<class UGameplayEffect> SprintEffect;

	bool bDoOnce = true;
};
