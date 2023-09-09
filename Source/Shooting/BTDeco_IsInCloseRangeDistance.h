// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDeco_IsInCloseRangeDistance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API UBTDeco_IsInCloseRangeDistance : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDeco_IsInCloseRangeDistance();
public:
	UPROPERTY(EditAnywhere)
		float Range;
public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
