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
			ControllingOwner->Attack(Target);
				
		}	

		BlackboardComp->SetValueAsFloat("LastServiceExecutionTime", CurrentTime);
	}

	return EBTNodeResult::Failed;
}
