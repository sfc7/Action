// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SkeletonAttack.h"
#include "MonsterSkeleton.h"
#include "BaseCharacter.h"
#include "MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SkeletonAttack::UBTTask_SkeletonAttack()
{
	NodeName = TEXT("SkeletonAttack");
}

EBTNodeResult::Type UBTTask_SkeletonAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingOwner = Cast<AMonsterSkeleton>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingOwner) {
		return EBTNodeResult::Failed;
	}
	auto Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetActorKey));
	if (nullptr == Target) {
		return EBTNodeResult::Failed;
	}

	if (!(ControllingOwner->IsDamaging) && !(ControllingOwner->IsAttacking)) {
		ControllingOwner->Attack(Target);
		return EBTNodeResult::Succeeded;
	}


	return EBTNodeResult::Failed;
}
