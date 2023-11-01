// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CreatureAnimInstance.h"
#include "MonsterSkeletonAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API UMonsterSkeletonAnimInstance : public UCreatureAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		UAnimMontage* AttackMontage1;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		UAnimMontage* AttackMontage2;
	UPROPERTY()
		TArray<UAnimMontage*> AttackMontageArray;
	UPROPERTY(BlueprintReadWrite)
		bool IsFocusingTarget;

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	int32 AttackMontageNum = 2;

	virtual void PlayAttackMontage() override;
};

