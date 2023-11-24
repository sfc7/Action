// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterDragonAnimInstance.h"



void UMonsterDragonAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	AttackMontageArray = { AttackMontage1, AttackMontage2, AttackMontage3};
	RangeAttackMontageArray = { RangeAttackMontage1, RangeAttackMontage2};
}

void UMonsterDragonAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UMonsterDragonAnimInstance::PlayAttackMontage()
{
	int32 num = FMath::RandRange(0, AttackMontageNum - 1);
	if (IsValid(AttackMontageArray[num])) {
		if (!Montage_IsPlaying(AttackMontageArray[num])) {
			Montage_Play(AttackMontageArray[num]);
		}
	}
}

void UMonsterDragonAnimInstance::PlayRangeAttackMontage()
{
	int32 num = FMath::RandRange(0, RangeAttackMontageNum - 1);
	if (IsValid(RangeAttackMontageArray[num])) {
		if (!Montage_IsPlaying(RangeAttackMontageArray[num])) {
			Montage_Play(RangeAttackMontageArray[num]);
		}
	}
}
