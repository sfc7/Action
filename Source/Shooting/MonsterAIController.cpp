// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "BaseCharacter.h"

const FName AMonsterAIController::HomePosKey(TEXT("HomePos"));
const FName AMonsterAIController::TargetPosKey(TEXT("TargetPos"));
const FName AMonsterAIController::TargetActorKey(TEXT("TargetActor"));
const FName AMonsterAIController::IsDamagingKey(TEXT("IsDamaging"));

AMonsterAIController::AMonsterAIController()
{
	SetPerceptionComponent(*CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception")));

	SightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 10000.f;
	SightConfig->LoseSightRadius = 5000.f;
	SightConfig->PeripheralVisionAngleDegrees = 360.f;
	SightConfig->AutoSuccessRangeFromLastSeenLocation = -1.f;
	SightConfig->PointOfViewBackwardOffset = 0.0f;
	SightConfig->NearClippingRadius = 200.f;
	SightConfig->SetMaxAge(0.0f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterAIController::HandleSightSense);
}

void AMonsterAIController::HandleSightSense(AActor* actor, FAIStimulus const Stimulus)
{
	if (Stimulus.Type == UAISense::GetSenseID(UAISense_Sight::StaticClass())) {
		if (Cast<ABaseCharacter>(actor)) {
			if (Stimulus.WasSuccessfullySensed()) {
				Target = actor;
				Blackboard->SetValueAsObject(AMonsterAIController::TargetActorKey, Target);
			}
		}
	}
}	

void AMonsterAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AMonsterAIController::SetIsDamaging(bool _IsDamaging)
{
	Blackboard->SetValueAsBool(AMonsterAIController::IsDamagingKey, _IsDamaging);
	UE_LOG(LogTemp,Log,TEXT("123"));
}

void AMonsterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AMonsterAIController::OnUnPossess()
{
	Super::OnUnPossess();
}


