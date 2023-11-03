// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDeco_IsMonsterAttacking.h"
#include "MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseMonster.h"

bool UBTDeco_IsMonsterAttacking::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn) return false;

	UE_LOG(LogTemp, Log, TEXT("%d"), ControllingPawn->IsAttacking);
	return ControllingPawn->IsAttacking;
}
