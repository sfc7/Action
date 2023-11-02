// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterAIController.h"
#include "Monster_Boss_AIController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API AMonster_Boss_AIController : public AMonsterAIController
{
	GENERATED_BODY()
public:
	AMonster_Boss_AIController();
public:

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

protected:
	virtual void BeginPlay() override;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

};
