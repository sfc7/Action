// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster_Dragon_AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseCharacter.h"

AMonster_Dragon_AIController::AMonster_Dragon_AIController()
{
	static::ConstructorHelpers::FObjectFinder<UBehaviorTree> Tree(TEXT("/Script/AIModule.BehaviorTree'/Game/Shooting/BluePrint/AI/BT_Dragon.BT_Dragon'"));
	if (Tree.Succeeded())
	{
		BehaviorTree = Tree.Object;
	}

	static::ConstructorHelpers::FObjectFinder<UBlackboardData> Board(TEXT("/Script/AIModule.BlackboardData'/Game/Shooting/BluePrint/AI/BB_Dragon.BB_Dragon'"));
	if (Board.Succeeded())
	{
		BlackboardData = Board.Object;
	}


}

void AMonster_Dragon_AIController::BeginPlay()
{
	Super::BeginPlay();

}


void AMonster_Dragon_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BlackboardData, BlackboardComp)) {
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (RunBehaviorTree(BehaviorTree))
		{
		}
	}
	this->Blackboard = BlackboardComp;
}

void AMonster_Dragon_AIController::OnUnPossess()
{
	Super::OnUnPossess();
}
