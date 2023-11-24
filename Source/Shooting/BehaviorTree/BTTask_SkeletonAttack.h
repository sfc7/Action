// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SkeletonAttack.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API UBTTask_SkeletonAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_SkeletonAttack();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
