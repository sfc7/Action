// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossAttack.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
    Melee = 0 UMETA(DisplayName = "Melee"),
    Ranged UMETA(DisplayName = "Ranged"),
    Special UMETA(DisplayName = "Special")
};

UCLASS()
class SHOOTING_API UBTTask_Boss_Attack : public UBTTaskNode
{
    GENERATED_BODY()
public:
    UBTTask_Boss_Attack();
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        EAttackType AttackType;
public:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};