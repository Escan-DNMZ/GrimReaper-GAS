// Fill out your copyright notice in the Description page of Project Settings.

#include "Ai/SeverogAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/Character.h"
#include <Actors/Enemy/WalkingDeads.h>
#include <Ai/BlackboardKeys.h>
#include "Ai/Ai_Tags.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

ASeverogAIController::ASeverogAIController(FObjectInitializer const& object_initializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/Ai/GrimReaper/GrimReaper_BT.GrimReaper_BT'"));
	if (obj.Succeeded())
	{
		btree = obj.Object;
	}
	behaviour_tree_component = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviourTree"));
	blackboard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));
	setup_perceptionSystem();


}

void ASeverogAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(btree);
	behaviour_tree_component->StartTree(*btree);
}

void ASeverogAIController::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);
	if (blackboard)
	{
		blackboard->InitializeBlackboard(*btree->BlackboardAsset);
	}
}

UBlackboardComponent* ASeverogAIController::get_blackboard() const
{
	return blackboard;
}

void ASeverogAIController::on_upadated_CanSeePlayer(AActor* updated_actors, FAIStimulus const stimulus)
{
	
	if (auto const ch = Cast<AWalkingDeads>(updated_actors))
	{
		get_blackboard()->SetValueAsBool(bb_keys::can_see_player, stimulus.WasSuccessfullySensed());
	}
	
	
}

void ASeverogAIController::on_updated(TArray<AActor*> const& updated_actors)
{
	for (size_t i = 0; i < updated_actors.Num(); ++i)
	{
		FActorPerceptionBlueprintInfo info;
		GetPerceptionComponent()->GetActorsPerception(updated_actors[i],info);
		for (size_t x = 0; x < info.LastSensedStimuli.Num(); ++x)
		{
			FAIStimulus const stim = info.LastSensedStimuli[x];
			if (stim.Tag == Tags::noise_tag)
			{
				get_blackboard()->SetValueAsBool(bb_keys::is_investing, stim.WasSuccessfullySensed());
				get_blackboard()->SetValueAsVector(bb_keys::Target_Location, stim.StimulusLocation);
			}
		}
	}
}

void ASeverogAIController::setup_perceptionSystem()
{
	sight_config = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (sight_config)
	{

		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		sight_config->SightRadius = 500.0f;
		sight_config->LoseSightRadius = sight_config->SightRadius + 25.0f;
		sight_config->PeripheralVisionAngleDegrees = 90.0f;
		sight_config->SetMaxAge(5.0f);
		sight_config->AutoSuccessRangeFromLastSeenLocation = 520.0f;
		sight_config->DetectionByAffiliation.bDetectEnemies = true;
		sight_config->DetectionByAffiliation.bDetectFriendlies = true;
		sight_config->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*sight_config->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ASeverogAIController::on_upadated_CanSeePlayer);
		GetPerceptionComponent()->ConfigureSense(*sight_config);
	}
	hear_config = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	if (hear_config)
	{

		hear_config->HearingRange = 30000.0f;
		hear_config->DetectionByAffiliation.bDetectEnemies = true;
		hear_config->DetectionByAffiliation.bDetectFriendlies = true;
			hear_config->DetectionByAffiliation.bDetectNeutrals = true;

		
		GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &ASeverogAIController::on_updated);
		GetPerceptionComponent()->ConfigureSense(*hear_config);


	}

}
