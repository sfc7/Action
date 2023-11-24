// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster_Boss_AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseCharacter.h"
#include "MonsterBoss.h"
#include "MonsterComponent.h"

AMonster_Boss_AIController::AMonster_Boss_AIController()
{
	static::ConstructorHelpers::FObjectFinder<UBehaviorTree> Tree(TEXT("/Script/AIModule.BehaviorTree'/Game/Shooting/BluePrint/AI/BT_Boss.BT_Boss'"));
	if (Tree.Succeeded())
	{
		BehaviorTree = Tree.Object;
	}

	static::ConstructorHelpers::FObjectFinder<UBlackboardData> Board(TEXT("/Script/AIModule.BlackboardData'/Game/Shooting/BluePrint/AI/BB_Boss.BB_Boss'"));
	if (Board.Succeeded())
	{
		BlackboardData = Board.Object;
	}
}

void AMonster_Boss_AIController::OnPossess(APawn* InPawn)
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

void AMonster_Boss_AIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AMonster_Boss_AIController::BeginPlay()
{
	Super::BeginPlay();
}

void AMonster_Boss_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AMonsterBoss* AIOwner = Cast<AMonsterBoss>(GetPawn());
	if (AIOwner) {
		UMonsterComponent* OwnerComponent = AIOwner->MonsterComponent;
		if (OwnerComponent)
		{
			Blackboard->SetValueAsFloat(AMonster_Boss_AIController::CurrentHpKey, OwnerComponent->GetHpRatio());
		}
	}
}

void AMonster_Boss_AIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	AMonsterBoss* MonsterBoss = Cast<AMonsterBoss>(GetPawn());
	if (!MonsterBoss) return;

	if (Result.Code == EPathFollowingResult::Success)
	{
		MonsterBoss->TeleportEnd();
	}
	TeleportRequestID = FAIRequestID::InvalidRequest;
}

