// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBossAnimInstance.h"

UMonsterBossAnimInstance::UMonsterBossAnimInstance()
{
}

void UMonsterBossAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UMonsterBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UMonsterBossAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(AttackStabMontage)) {
		Montage_Play(AttackStabMontage);
	}
}

void UMonsterBossAnimInstance::PlayRangeAttackMontage()
{
	if (!Montage_IsPlaying(AttackFireBallMontage)) {
		Montage_Play(AttackFireBallMontage);
	}
}


