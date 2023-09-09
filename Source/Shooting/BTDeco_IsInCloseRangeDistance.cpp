// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDeco_IsInCloseRangeDistance.h"
#include "BaseCharacter.h"
#include "MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDeco_IsInCloseRangeDistance::UBTDeco_IsInCloseRangeDistance()
{
	NodeName = TEXT("IsInCloseRangeDistance");
	Range = 100.0f;
}

bool UBTDeco_IsInCloseRangeDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return false;


	auto Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetActorKey));
	if (nullptr == Target) return false;


	auto flag = Target->GetDistanceTo(ControllingPawn) <= Range;
	return flag;
}
