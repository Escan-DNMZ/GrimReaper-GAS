	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "SeverogAIController.generated.h"

/**
 * 
 */
UCLASS()
class GASDEMO2_API ASeverogAIController : public AAIController
{
	GENERATED_BODY()
public:
	ASeverogAIController(FObjectInitializer const& object_initializer = FObjectInitializer::Get());
	void BeginPlay() override;
	void OnPossess(APawn* const pawn) override;
	class UBlackboardComponent* get_blackboard() const;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ai", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTreeComponent* behaviour_tree_component;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ai", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree* btree;

	class UBlackboardComponent* blackboard;

	class UAISenseConfig_Sight* sight_config;

	class UAISenseConfig_Hearing* hear_config;

	UFUNCTION()
		void on_upadated_CanSeePlayer(AActor* updated_actors,FAIStimulus const stimulus);

	UFUNCTION()
		void on_updated(TArray<AActor*> const& updated_actors);

	void setup_perceptionSystem();


};
