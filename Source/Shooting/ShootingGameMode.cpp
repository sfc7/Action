// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShootingGameMode.h"
#include "KatanaCharacter.h"
#include "BasePlayerController.h"
#include "Kismet/GameplayStatics.h"

AShootingGameMode::AShootingGameMode()
{

	auto DefaultCharacter = AKatanaCharacter::StaticClass();

	if (IsValid(DefaultCharacter))
	{
		DefaultPawnClass = DefaultCharacter;
	}

	auto DefaultController = ABasePlayerController::StaticClass();

	if (IsValid(DefaultController)) {
		PlayerControllerClass = DefaultController;
	}
}

void AShootingGameMode::GameOver(APawn* Pawn)
{
	APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController());
	if(PlayerController != nullptr) {
		PlayerController->GameHasEnded(UGameplayStatics::GetPlayerCharacter(GetWorld(),0), false);
	}
}
