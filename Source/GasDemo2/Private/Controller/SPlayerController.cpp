// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/SPlayerController.h"
#include <Blueprint/UserWidget.h>

//
//void ASPlayerController::Hud()
//{
//	HudClassInstance = CreateWidget<UUserWidget>(this, HudClass);
//	if (HudClassInstance)
//	{
//		HudClassInstance->AddToViewport(100);
//	}
//}


void ASPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}

void ASPlayerController::BeginPlayingState()
{
	BlueprintBeginPlayingState();
}

void ASPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	
	OnPlayerStateReceived.Broadcast(PlayerState);
}

