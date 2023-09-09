// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Trigger_Stage1_Event1.generated.h"


/**
 * 
 */
UCLASS()
class SHOOTING_API ATrigger_Stage1_Event1 : public ATriggerBox
{
	GENERATED_BODY()
public:
	ATrigger_Stage1_Event1();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere)
		TSubclassOf<class AActor> StaticNoSignWall_1;
	UPROPERTY(VisibleAnywhere)
		TSubclassOf<class AActor> StaticNoSignWall_2;
	UPROPERTY(VisibleAnywhere)
		TSubclassOf<class AActor> StaticDirectionArrow1;
	UPROPERTY(VisibleAnywhere)
		TSubclassOf<class AActor> StaticDirectionArrow2;
	UPROPERTY(VisibleAnywhere)
		class AActor* NoSignWall_1;
	UPROPERTY(VisibleAnywhere)
		class AActor* NoSignWall_2;
	UPROPERTY(VisibleAnywhere)
		class AActor* DirectionArrow1;
	UPROPERTY(VisibleAnywhere)
		class AActor* DirectionArrow2;

	UFUNCTION()
	void Event(class AActor* OverlapActor, class AActor* OtherActor);
	void OnMonsterDeath();

	bool IsFlag = false;

};
