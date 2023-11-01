// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSkeletonAnimInstance.h"
#include "Monster_Skeleton_AIController.h"

void UMonsterSkeletonAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	AttackMontageArray = { AttackMontage1, AttackMontage2 };
}

void UMonsterSkeletonAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* Owner = Cast<APawn>(GetOwningActor());
	if (IsValid(Owner)) {
		auto AIController = Cast<AMonsterAIController>(Owner->GetController());
		if (AIController) {
			if (IsValid(AIController->GetFocusActor())) {
				IsFocusingTarget = true;
			}
		}
	}
}

void UMonsterSkeletonAnimInstance::PlayAttackMontage()
{
	int32 num = FMath::RandRange(0, AttackMontageNum-1);
	if (IsValid(AttackMontageArray[num])) {
		if (!Montage_IsPlaying(AttackMontageArray[num])) {
			Montage_Play(AttackMontageArray[num]);
		}
	}
}
