// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShootingGameMode.h"
#include "KatanaCharacter.h"

AShootingGameMode::AShootingGameMode()
{

	auto DefaultCharacter = AKatanaCharacter::StaticClass();

	if (IsValid(DefaultCharacter))
	{
		DefaultPawnClass = DefaultCharacter;
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.
//
//#include "ShootingGameMode.h"
//#include "ShootingCharacter.h"
//#include "UObject/ConstructorHelpers.h"
//
//AShootingGameMode::AShootingGameMode()
//{
//	// set default pawn class to our Blueprinted character
//	static ConstructorHelpers::FClassFinder<APawn> Player(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
//	if (Player.Succeeded())
//	{
//		DefaultPawnClass = Player.Class;
//	}
//}
