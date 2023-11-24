// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Trigger_Stage1_Event2.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API ATrigger_Stage1_Event2 : public ATriggerBox
{
	GENERATED_BODY()
public:
	ATrigger_Stage1_Event2();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere)
		TSubclassOf<class AActor> StaticNoSignWall_3;
	UPROPERTY(VisibleAnywhere)
		TSubclassOf<class AActor> StaticDirectionArrow1;
	UPROPERTY(VisibleAnywhere)
		class AActor* NoSignWall_3;
	UPROPERTY(VisibleAnywhere)
		class AActor* DirectionArrow1;

	UFUNCTION()
		void Event(class AActor* OverlapActor, class AActor* OtherActor);
	void OnMonsterDeath();

	bool IsFlag = false;

};