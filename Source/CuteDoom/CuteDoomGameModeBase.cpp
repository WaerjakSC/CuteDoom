// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "CuteDoomGameModeBase.h"
#include "UObject/ConstructorHelpers.h"

ACuteDoomGameModeBase::ACuteDoomGameModeBase()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(
		TEXT("/Game/Blueprints/Player/BP_CuteCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	// set default player controller to our blueprinted controller.
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassFinder(
		TEXT("/Game/Blueprints/Player/BP_CuteController"));
	PlayerControllerClass = PlayerControllerClassFinder.Class;
}


