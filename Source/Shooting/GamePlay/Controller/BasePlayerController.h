// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ABasePlayerController();
public:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> ScreenLoseText;
public:
	virtual void GameHasEnded(class AActor* EndGameFocus, bool bIsWinner);
};
