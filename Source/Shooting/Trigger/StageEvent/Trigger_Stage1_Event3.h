// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Trigger_Stage1_Event3.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API ATrigger_Stage1_Event3 : public ATriggerBox
{
	GENERATED_BODY()
public:
	ATrigger_Stage1_Event3();
protected:
	virtual void BeginPlay() override;
public:
	UFUNCTION()
		void Event(class AActor* OverlapActor, class AActor* OtherActor);
};
