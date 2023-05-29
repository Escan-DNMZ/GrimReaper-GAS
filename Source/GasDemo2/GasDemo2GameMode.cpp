// Copyright Epic Games, Inc. All Rights Reserved.

#include "GasDemo2GameMode.h"
#include "GasDemo2Character.h"
#include "UObject/ConstructorHelpers.h"

AGasDemo2GameMode::AGasDemo2GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
