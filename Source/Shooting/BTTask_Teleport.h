// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Teleport.generated.h"

class AMonsterBoss;
/**
 * 
 */
UCLASS()
class SHOOTING_API UBTTask_Teleport : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_Teleport();
public:
	UPROPERTY(VisibleAnywhere)
		class AMonsterBoss* Owner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FBlackboardKeySelector TeleportLocation;
public:
	void OnTeleportEnd();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
