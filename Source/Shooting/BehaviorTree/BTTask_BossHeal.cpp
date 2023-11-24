// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BossHeal.h"
#include "MonsterBossAnimInstance.h"
#include "MonsterBoss.h"
#include "MonsterBoss.h"
#include "MonsterAIController.h"
UBTTask_BossHeal::UBTTask_BossHeal()
{
	NodeName = TEXT("BossHeal");
}

EBTNodeResult::Type UBTTask_BossHeal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingOwner = Cast<AMonsterBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingOwner) {
		return EBTNodeResult::Failed;
	}
	auto Anim = Cast<UMonsterBossAnimInstance>(ControllingOwner->GetMesh()->GetAnimInstance());
	if (IsValid(Anim)) {
		Anim->PlayHealMontage();
	}
	else {
		return EBTNodeResult::Failed;
	}



	return EBTNodeResult::Type();
}
