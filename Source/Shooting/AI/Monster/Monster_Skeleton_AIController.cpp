// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster_Skeleton_AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
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

void AMonster_Skeleton_AIController::BeginPlay()
{
	Super::BeginPlay();


}
