// Fill out your copyright notice in the Description page of Project Settings.


#include "GunAnimInstance.h"
#include "GunCharacter.h"
#include "Math/UnrealMathUtility.h"

void UGunAnimInstance::AnimNotify_Pause()
{	
	if (Montage_IsPlaying(Attack_R_SkillMontage)) {
		Montage_Pause(Attack_R_SkillMontage);
	}
}

void UGunAnimInstance::AnimNotify_AbilityREnd()
{
	AbilityREnd.Broadcast();
}

void UGunAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	if(IsValid(Creature)) {
		GunCreature = Cast<AGunCharacter>(Creature);
	}
	
}

void UGunAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(GunCreature)) {
		IsAiming = GunCreature->IsAiming;
		
		if (GunCreature->IsAttacking_R_Skill) {
			IsAttackingRSkill = true;
			PrevRotation = CurRotation;
			CurRotation = GunCreature->GetActorRotation();
			RSkillDeltaRotation = CurRotation - PrevRotation;
			RSkillDeltaRotation.Normalize();
			RotationYaw -= RSkillDeltaRotation.Yaw;
			RotationYaw = FMath::Clamp(RotationYaw, -90.0f, 90.0f);
	
		}
		else {
			IsAttackingRSkill = false;
			RotationYaw = 0.0f;
		}

	}
}
