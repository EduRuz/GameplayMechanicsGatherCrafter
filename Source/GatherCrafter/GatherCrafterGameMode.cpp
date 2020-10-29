// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "GatherCrafterGameMode.h"
#include "GatherCrafterCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGatherCrafterGameMode::AGatherCrafterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
