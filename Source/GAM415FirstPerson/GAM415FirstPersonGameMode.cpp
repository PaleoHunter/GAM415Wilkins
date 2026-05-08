// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAM415FirstPersonGameMode.h"
#include "GAM415FirstPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGAM415FirstPersonGameMode::AGAM415FirstPersonGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
