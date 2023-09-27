// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Creature) {
		ControllerRotation = Creature->GetControlRotation();
		ActorRotation = Creature->GetActorRotation();
		DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(ControllerRotation, ActorRotation);
	}

}

void UCharacterAnimInstance::AnimNotify_NextAttack()
{
	AttackComboCheck.Broadcast();
}

void UCharacterAnimInstance::AnimNotify_AttackMontageEnd()
{
	AttackMontageEnd.Broadcast();
}

void UCharacterAnimInstance::JumpToAttackMontageSection(int Section)
{
	if (Montage_IsPlaying(AttackMontage)) {
		Montage_JumpToSection(GetAttackMontageSectionName(Section), AttackMontage);
	}
}



void UCharacterAnimInstance::PlayAttackMontage()
{
	if (IsValid(AttackMontage)) {
		Montage_Play(AttackMontage);
	}
}

void UCharacterAnimInstance::PlayAttack_Q_SkillMontage()
{
	if (IsValid(Attack_Q_SkillMontage))
	{
		if (!Montage_IsPlaying(Attack_Q_SkillMontage)) {

			Montage_Play(Attack_Q_SkillMontage);
		}
	}
}

void UCharacterAnimInstance::PlayAttack_R_SkillMontage()
{
	if (IsValid(Attack_R_SkillMontage))
	{
		if (!Montage_IsPlaying(Attack_R_SkillMontage)) {

			Montage_Play(Attack_R_SkillMontage);
		}
	}
}

void UCharacterAnimInstance::PlayForwardRollMontage()
{
	if (IsValid(ForwardRollMontage)) {
		if (!Montage_IsPlaying(ForwardRollMontage)) {
			Montage_Play(ForwardRollMontage);
		}
	}
}

FName UCharacterAnimInstance::GetAttackMontageSectionName(int Section)
{
	return FName(*FString::Printf(TEXT("Attack%d"),Section));
}





