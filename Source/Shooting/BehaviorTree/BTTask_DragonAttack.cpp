// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DragonAttack.h"
#include "MonsterDragon.h"
#include "BaseCharacter.h"
#include "MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTTask_DragonAttack::UBTTask_DragonAttack()
{
	NodeName = TEXT("DragonAttack");
}

EBTNodeResult::Type UBTTask_DragonAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingOwner = Cast<AMonsterDragon>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingOwner) {
		return EBTNodeResult::Failed;
	}
	auto Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetActorKey));
	if (nullptr == Target) {
		return EBTNodeResult::Failed;
	}

	switch (AttackType) {
	case EDragonAttackType::Melee:
		if (!(ControllingOwner->IsDamaging) && !(ControllingOwner->IsAttacking)) {
			ControllingOwner->Attack(Target);
			return EBTNodeResult::Succeeded;
		}
		break;
	case EDragonAttackType::Ranged:
		if (!(ControllingOwner->IsDamaging) && !(ControllingOwner->IsAttacking)) {
			ControllingOwner->RangeAttack(Target);
			return EBTNodeResult::Succeeded;
		}
		break;
	}

	return EBTNodeResult::Failed;
}
