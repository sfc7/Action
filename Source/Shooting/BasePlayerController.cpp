// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerController.h"
#include "TimerManager.h"

ABasePlayerController::ABasePlayerController()
{
}

void ABasePlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);


	FTimerHandle waitHandle;
	GetWorld()->GetTimerManager().SetTimer(waitHandle, this, &APlayerController::RestartLevel, 6.0f, false);
}
