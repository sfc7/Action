// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBossAnimInstance.h"

void UMonsterBossAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	AttackMontageArray = { AttackStabMontage1, AttackStabMontage2 };
	RangeAttackMontageArray = { AttackFireBallMontage,AttackMultipleFireBallMontage, GateofBabylonMontage,AttackChargeMontage };
}

void UMonsterBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UMonsterBossAnimInstance::PlayAttackMontage()
{
	int32 num = FMath::RandRange(0, AttackMontageNum - 1);
	if (IsValid(AttackMontageArray[num])) {
		if (!Montage_IsPlaying(AttackMontageArray[num])) {
			Montage_Play(AttackMontageArray[num]);
		}
	}
}

void UMonsterBossAnimInstance::PlayRangeAttackMontage()
{
	int32 num = FMath::RandRange(0, RangeAttackMontageNum - 1);
	if (IsValid(RangeAttackMontageArray[num])) {
		if (!Montage_IsPlaying(RangeAttackMontageArray[num])) {
			Montage_Play(RangeAttackMontageArray[num]);
		}
	}
}


