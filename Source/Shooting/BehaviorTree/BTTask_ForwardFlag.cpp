// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ForwardFlag.h"
#include "Monster_Skeleton_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ForwardFlag::UBTTask_ForwardFlag()
{
    NodeName = TEXT("ForwardFlag");
}

EBTNodeResult::Type UBTTask_ForwardFlag::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMonster_Skeleton_AIController::ForwardFlagKey, true);


    return EBTNodeResult::Succeeded;
}
