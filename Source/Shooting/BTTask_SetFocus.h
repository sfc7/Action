// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetFocus.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API UBTTask_SetFocus : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_SetFocus();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBlackboardKeySelector TargetKey;

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
