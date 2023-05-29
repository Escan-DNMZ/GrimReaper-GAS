// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/FindPlayerLocation.h"
#include "GameFramework/Character.h"
#include "Ai/FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "Ai/SeverogAIController.h"
//#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Ai/BlackboardKeys.h"
#include <Kismet/GameplayStatics.h>

UFindPlayerLocation::UFindPlayerLocation(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("FindPlayerLocation");
}

EBTNodeResult::Type UFindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memoery)
{
	const ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	const auto cont = Cast<ASeverogAIController>(owner_comp.GetAIOwner());

	const FVector player_Location = player->GetActorLocation();
	
	if (cont->get_blackboard()->GetValueAsBool(bb_keys::can_see_player))
	{
		cont->get_blackboard()->SetValueAsVector(bb_keys::Target_Location, player_Location);
	}
	else
	{
		FNavLocation loc;

		const UNavigationSystemV1* nav_sys = UNavigationSystemV1::GetCurrent(GetWorld());
		if (nav_sys->GetRandomPointInNavigableRadius(player_Location, search_radius, loc, nullptr))
		{
			cont->get_blackboard()->SetValueAsVector(bb_keys::Target_Location, loc.Location);
		}
	}

	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
