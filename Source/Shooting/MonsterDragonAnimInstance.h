// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CreatureAnimInstance.h"
#include "MonsterDragonAnimInstance.generated.h"
DECLARE_MULTICAST_DELEGATE(FAttackHitType);

/**
 * 
 */
UCLASS()
class SHOOTING_API UMonsterDragonAnimInstance : public UCreatureAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		UAnimMontage* AttackMontage1;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		UAnimMontage* AttackMontage2;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		UAnimMontage* AttackMontage3;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		UAnimMontage* RangeAttackMontage1;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		UAnimMontage* RangeAttackMontage2;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		UAnimMontage* AttackMontage6;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		TArray<UAnimMontage*> AttackMontageArray;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		TArray<UAnimMontage*> RangeAttackMontageArray;

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
public:
	int32 AttackMontageNum = 3;
	int32 RangeAttackMontageNum = 2;

	virtual void PlayAttackMontage() override;
	virtual void PlayRangeAttackMontage() override;

};
 