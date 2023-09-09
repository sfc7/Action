// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster_Skeleton_AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "BaseCharacter.h"

const FName AMonster_Skeleton_AIController::ForwardLocationKey(TEXT("ForwardLocation"));
const FName AMonster_Skeleton_AIController::ForwardFlagKey(TEXT("ForwardFlag"));

AMonster_Skeleton_AIController::AMonster_Skeleton_AIController()
{
	static::ConstructorHelpers::FObjectFinder<UBehaviorTree> Tree(TEXT("/Script/AIModule.BehaviorTree'/Game/Shooting/BluePrint/AI/BT_Skeleton.BT_Skeleton'"));
	if (Tree.Succeeded())
	{
		BehaviorTree = Tree.Object;
	}

	static::ConstructorHelpers::FObjectFinder<UBlackboardData> Board(TEXT("/Script/AIModule.BlackboardData'/Game/Shooting/BluePrint/AI/BB_Skeleton.BB_Skeleton'"));
	if (Board.Succeeded())
	{
		BlackboardData = Board.Object;
	}

	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AMonster_Skeleton_AIController::HandleSightSense);
}

void AMonster_Skeleton_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BlackboardData, BlackboardComp)) {
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		Blackboard->SetValueAsVector(ForwardLocationKey, InPawn->GetActorLocation() + InPawn->GetActorRotation().Vector() * 1000.0f);
		Blackboard->SetValueAsBool(ForwardFlagKey, false);
		if (RunBehaviorTree(BehaviorTree))
		{
		}
	}
	this->Blackboard = BlackboardComp;
}

void AMonster_Skeleton_AIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AMonster_Skeleton_AIController::HandleSightSense(AActor* actor, FAIStimulus const Stimulus)
{
				
	if (Stimulus.Type == UAISense::GetSenseID(UAISense_Sight::StaticClass())) {
		if (auto const player = Cast<ABaseCharacter>(actor)) {

			if (Stimulus.WasSuccessfullySensed()) {
				Blackboard->SetValueAsObject(AMonster_Skeleton_AIController::TargetActorKey, player);

			}
			else {
				Blackboard->SetValueAsVector(AMonster_Skeleton_AIController::LastKnownLocationKey, Stimulus.StimulusLocation);
			}
		}
		else {
			return;
		}
	}
	else {
		return;
	}

}