// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "CuteDoomGameModeBase.h"
#include "UObject/ConstructorHelpers.h"

ACuteDoomGameModeBase::ACuteDoomGameModeBase()
{
		// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/Player/BP_CuteCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
