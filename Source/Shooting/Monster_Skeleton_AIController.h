// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterAIController.h"
#include "Monster_Skeleton_AIController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API AMonster_Skeleton_AIController : public AMonsterAIController
{
	GENERATED_BODY()
public:
	AMonster_Skeleton_AIController();
public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

protected:
	virtual void BeginPlay() override;
public:
	static const FName ForwardLocationKey;
	static const FName ForwardFlagKey;
};
