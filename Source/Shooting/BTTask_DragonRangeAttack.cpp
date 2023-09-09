// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DragonRangeAttack.h"
#include "MonsterDragon.h"
#include "BaseCharacter.h"
#include "MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTTask_DragonRangeAttack::UBTTask_DragonRangeAttack()
{
	NodeName = TEXT("DragonRangeAttack");
}

EBTNodeResult::Type UBTTask_DragonRangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	float CurrentTime = UGameplayStatics::GetRealTimeSeconds(OwnerComp.GetWorld());

	float LastServiceExecutionTime = BlackboardComp->GetValueAsFloat("LastServiceExecutionTime");


	if (CurrentTime - LastServiceExecutionTime >= 4.0f)
	{
		auto ControllingOwner = Cast<AMonsterDragon>(OwnerComp.GetAIOwner()->GetPawn());
		if (nullptr == ControllingOwner) {
			return EBTNodeResult::Failed;
		}
		auto Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetActorKey));
		if (nullptr == Target) {
			return EBTNodeResult::Failed;
		}

		if (!(ControllingOwner->IsAttacking)) {
			ControllingOwner->RangeAttack(Target);
		}

		BlackboardComp->SetValueAsFloat("LastServiceExecutionTime", CurrentTime);
	}

	return EBTNodeResult::Failed;
}
