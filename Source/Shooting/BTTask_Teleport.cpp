// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Teleport.h"
#include "MonsterBoss.h"
#include "Monster_Boss_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Teleport::UBTTask_Teleport()
{
	NodeName = TEXT("Teleport");
}


EBTNodeResult::Type UBTTask_Teleport::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController) {
		Owner = Cast<AMonsterBoss>(AIController->GetPawn());
		if (Owner) {
			Owner->Teleport(OwnerComp.GetBlackboardComponent()->GetValueAsVector(TeleportLocation.SelectedKeyName));

			return EBTNodeResult::Succeeded;
		}

	}


	return EBTNodeResult::Failed;
}
