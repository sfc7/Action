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
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		UAnimMontage* AttackStabMontage1;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		UAnimMontage* AttackStabMontage2;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		UAnimMontage* AttackFireBallMontage;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		UAnimMontage* AttackMultipleFireBallMontage;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		UAnimMontage* GateofBabylonMontage;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		UAnimMontage* AttackChargeMontage;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		UAnimMontage* HealMontage;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		TArray<UAnimMontage*> AttackMontageArray;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadOnly)
		TArray<UAnimMontage*> RangeAttackMontageArray;

protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
public:
	int32 AttackMontageNum = 2;
	int32 RangeAttackMontageNum = 4;

	virtual void PlayAttackMontage() override;
	void PlayHealMontage();
	virtual void PlayRangeAttackMontage() override;
};
