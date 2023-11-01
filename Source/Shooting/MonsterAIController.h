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
public:
	UPROPERTY()
		UBlackboardData* BlackboardData;
	UPROPERTY()
		UBehaviorTree* BehaviorTree;
	UPROPERTY()
		UBlackboardComponent* BlackboardComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Target;

public:
	static const FName HomePosKey;
	static const FName TargetPosKey;
	static const FName TargetActorKey;
	static const FName IsDamagingKey;
	FVector HomePosition;
	class UAISenseConfig_Sight* SightConfig;

protected:
	virtual void BeginPlay() override;
public:
	UFUNCTION()
		void HandleSightSense(AActor* actor, FAIStimulus const Stimulus);

	UFUNCTION()
	void SetIsDamaging(bool _IsDamaging);

	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;


};
