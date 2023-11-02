// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CreatureAnimInstance.h"
#include "MonsterBossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API UMonsterBossAnimInstance : public UCreatureAnimInstance
{
	GENERATED_BODY()
public:
	UMonsterBossAnimInstance();
public:
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		UAnimMontage* AttackMontage1;
protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	virtual void PlayAttackMontage() override;
};
