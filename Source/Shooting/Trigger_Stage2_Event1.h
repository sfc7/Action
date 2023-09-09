// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Trigger_Stage2_Event1.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API ATrigger_Stage2_Event1 : public ATriggerBox
{
	GENERATED_BODY()
public:
	ATrigger_Stage2_Event1();

protected:
	virtual void BeginPlay() override;
public:
	UFUNCTION()
		void Event(class AActor* OverlapActor, class AActor* OtherActor);

	bool IsFlag = false;
};
