// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DragonAttack.generated.h"

UENUM(BlueprintType)
enum class EDragonAttackType : uint8
{
	Melee = 0 UMETA(DisplayName = "Melee"),
	Ranged UMETA(DisplayName = "Ranged"),
};

/**
 * 
 */
UCLASS()
class SHOOTING_API UBTTask_DragonAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_DragonAttack();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EDragonAttackType AttackType;
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
