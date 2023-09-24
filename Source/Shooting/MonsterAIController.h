// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "MonsterAIController.generated.h"


/**
 * 
 */
UCLASS()
class SHOOTING_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()
public:
	AMonsterAIController();
protected:
	UPROPERTY()
		UBlackboardData* BlackboardData;
	UPROPERTY()
		UBehaviorTree* BehaviorTree;
	UPROPERTY()
		UBlackboardComponent* BlackboardComp;
public:
	static const FName HomePosKey;
	static const FName TargetPosKey;
	static const FName TargetActorKey;
	FVector HomePosition;

	class UAISenseConfig_Sight* SightConfig;

	UFUNCTION()
		void HandleSightSense(AActor* actor, FAIStimulus const Stimulus);
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;


};
