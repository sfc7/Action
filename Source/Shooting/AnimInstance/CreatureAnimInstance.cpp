		// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatureAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"




void UCreatureAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn)) {
		Creature = Cast<ACharacter>(Pawn);
	}

	if (IsValid(Creature))
	{
		CharacterMovement = Creature->GetCharacterMovement();
	}

}

void UCreatureAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (IsValid(CharacterMovement))
	{
		Velocity = CharacterMovement->Velocity;
		GroundSpeed = Velocity.Size2D();
		auto Acceleration = CharacterMovement->GetCurrentAcceleration();
		ShouldMove = (GroundSpeed > 3.0f);
		IsFalling = CharacterMovement->IsFalling();
		Direction = CalculateDirection(Velocity, GetOwningActor()->GetActorRotation());
	}
}


void UCreatureAnimInstance::PlayHitReactMontage()
{
	if (IsValid(HitReactMontage)) {
		Montage_Play(HitReactMontage);
	}
}

void UCreatureAnimInstance::PlayDeathMontage()
{
	if (IsValid(DeathMontage)) {
		Montage_Play(DeathMontage, 1.0f, EMontagePlayReturnType::Duration, 0.0f);
	}
}

void UCreatureAnimInstance::AnimNotify_DisableMovement()
{
	AttackMovement.Broadcast(false);

}

void UCreatureAnimInstance::AnimNotify_EnableMovement()
{
	AttackMovement.Broadcast(true);
}

void UCreatureAnimInstance::AnimNotify_DisableAboutDodge()
{
	DodgeMovement.Broadcast(false);
}

void UCreatureAnimInstance::AnimNotify_EnableAboutDodge()
{
	DodgeMovement.Broadcast(true);
}

void UCreatureAnimInstance::AnimNotify_DisableDuringHit()
{
	OnHit.Broadcast(false);
}

void UCreatureAnimInstance::AnimNotify_EnableDuringHit()
{
	OnHit.Broadcast(true);
}

void UCreatureAnimInstance::PlayAttackMontage()
{
}

void UCreatureAnimInstance::PlayRangeAttackMontage()
{
}


void UCreatureAnimInstance::AnimNotify_DisableDuringAttack()
{
	DuringAttack.Broadcast(false);
	
}

void UCreatureAnimInstance::AnimNotify_EnableDuringAttack()
{
	DuringAttack.Broadcast(true);
}

void UCreatureAnimInstance::AnimNotify_DisableDuringDodge()
{
	DuringDodge.Broadcast(false);
}

void UCreatureAnimInstance::AnimNotify_EnableDuringDodge()
{
	DuringDodge.Broadcast(true);
}

void UCreatureAnimInstance::AnimNotify_BasicHit()
{
	AttackBasic.Broadcast();
}

void UCreatureAnimInstance::AnimNotify_QSkillHit()
{
	AttackQSkill.Broadcast();
}

void UCreatureAnimInstance::AnimNotify_RskillHit()
{
	AttackRSkill.Broadcast();
}



void UCreatureAnimInstance::AnimNotify_Fireball()
{
	AttackFireball.Broadcast();
}

void UCreatureAnimInstance::AnimNotify_MultipleFireball()
{
	AttackMultipleFireball.Broadcast();
}
void UCreatureAnimInstance::AnimNotify_GateofBabylon()
{
	AttackGateofBabylon.Broadcast();
}

void UCreatureAnimInstance::AnimNotify_Firebreath()
{
	AttackFirebreath.Broadcast();
}

void UCreatureAnimInstance::AnimNotify_AttackBite()
{
	AttackBite.Broadcast();
}

void UCreatureAnimInstance::AnimNotify_AttackLClaw()
{
	AttackLClaw.Broadcast();
}

void UCreatureAnimInstance::AnimNotify_AttackRClaw()
{
	AttackRClaw.Broadcast();
}

void UCreatureAnimInstance::AnimNotify_SpawnWeapon_l()
{
	SpawnWeapon_l.Broadcast();
}

void UCreatureAnimInstance::AnimNotify_SpawnWeapon_r()
{
	SpawnWeapon_r.Broadcast();
}

void UCreatureAnimInstance::AnimNotify_SpawnWeapon_Spine()
{
	SpawnWeapon_spine.Broadcast();
}

void UCreatureAnimInstance::AnimNotify_DestroyWeapon()
{
	destroyWeapon.Broadcast();
}

void UCreatureAnimInstance::AnimNotify_AttackBasicSound()
{
	AttackBasicSound.Broadcast();
}

void UCreatureAnimInstance::AnimNotify_AttackBasicSound2()
{
	AttackBasicSound2.Broadcast();
}

void UCreatureAnimInstance::AnimNotify_AttackBasicSound3()
{
	AttackBasicSound3.Broadcast();
}

void UCreatureAnimInstance::AnimNotify_AttackBasicSound4()
{
	AttackBasicSound4.Broadcast();
}

void UCreatureAnimInstance::AnimNotify_AttackRSkillSound()
{
	AttackRSkillSound.Broadcast();
}

void UCreatureAnimInstance::AnimNotify_Heal()
{
	Heal.Broadcast();
}
