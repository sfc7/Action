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

	if (!Montage_IsPlaying(AttackMontage1)) {
		Montage_Play(AttackMontage1);
	}
}
