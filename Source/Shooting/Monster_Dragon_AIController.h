// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterAIController.h"
#include "Monster_Dragon_AIController.generated.h"

/**
 *
 */
UCLASS()
class SHOOTING_API AMonster_Dragon_AIController : public AMonsterAIController
{
	GENERATED_BODY()
public:
	AMonster_Dragon_AIController();
protected:
	virtual void BeginPlay() override;
public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

};
