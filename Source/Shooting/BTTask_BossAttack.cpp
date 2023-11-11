// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BossAttack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseCharacter.h"
#include "MonsterBoss.h"
#include "MonsterAIController.h"

UBTTask_Boss_Attack::UBTTask_Boss_Attack()
{
    NodeName = TEXT("BossAttack");
}

EBTNodeResult::Type UBTTask_Boss_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingOwner = Cast<AMonsterBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingOwner) {
		return EBTNodeResult::Failed;
	}
	auto Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetActorKey));
	if (nullptr == Target) {
		return EBTNodeResult::Failed;
	}

	switch (AttackType) {
	case EAttackType::Melee:
		if (!(ControllingOwner->IsDamaging) && !(ControllingOwner->IsAttacking)) {
			ControllingOwner->Attack(Target);
			return EBTNodeResult::Succeeded;
		}
		break;
	case EAttackType::Ranged:
		if (!(ControllingOwner->IsDamaging) && !(ControllingOwner->IsAttacking)) {
			ControllingOwner->RangeAttack(Target);
			return EBTNodeResult::Succeeded;
		}
		break;
	}
    
  

    return EBTNodeResult::Succeeded;
}
