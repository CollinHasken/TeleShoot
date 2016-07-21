// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "TeleShoot.h"
#include "TeleShootGameMode.h"
#include "TeleShootCharacter.h"
#include "Control.h"

ATeleShootGameMode::ATeleShootGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/MyTeleShootCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PlayerControllerClass = AControl::StaticClass();
}
