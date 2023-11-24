// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Trigger_Stage1_Start.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API ATrigger_Stage1_Start : public ATriggerBox
{
	GENERATED_BODY()
public:
	ATrigger_Stage1_Start();
protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere)
		TSubclassOf<class AActor> StaticDirectionArrow;
	UPROPERTY(VisibleAnywhere)
		class AActor* DirectionArrow;


	UFUNCTION()
		void Event(class AActor* OverlapActor, class AActor* OtherActor);
	UFUNCTION()
		void EndEvent(class AActor* OverlapActor, class AActor* OtherActor);

	bool IsFlag = false;
};
