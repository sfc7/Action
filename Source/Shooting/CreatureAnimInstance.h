// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CreatureAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FAttackMovement, bool)
DECLARE_MULTICAST_DELEGATE_OneParam(FDodgeMovement, bool)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHit, bool)
DECLARE_MULTICAST_DELEGATE_OneParam(FDuringAttack, bool)
DECLARE_MULTICAST_DELEGATE_OneParam(FDuringDodge, bool)
DECLARE_MULTICAST_DELEGATE(FAttackHitType);
DECLARE_MULTICAST_DELEGATE(FSoundType);
/**
 *
 */
UCLASS()
class SHOOTING_API UCreatureAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(Category = Movement, VisibleAnywhere, BlueprintReadWrite)
		bool ShouldMove;
	UPROPERTY(Category = Movement, VisibleAnywhere, BlueprintReadWrite)
		bool IsFalling;
	UPROPERTY(Category = Movement, VisibleAnywhere, BlueprintReadWrite)
		float GroundSpeed;
	UPROPERTY(Category = Movement, VisibleAnywhere, BlueprintReadWrite)
		FVector Velocity;	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float Direction;

public:
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadWrite)
		UAnimMontage* HitReactMontage;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadWrite)
		UAnimMontage* DeathMontage;
public:
	UPROPERTY(VisibleAnywhere)
		class ACharacter* Creature;
	UPROPERTY(VisibleAnywhere)
		class UCharacterMovementComponent* CharacterMovement;
public:
	FAttackMovement AttackMovement;
	FDodgeMovement DodgeMovement;
	FOnHit OnHit;
	FDuringAttack DuringAttack;
	FDuringDodge DuringDodge;
	FAttackHitType AttackBasic;
	FAttackHitType AttackQSkill;
	FAttackHitType AttackRSkill;
	FAttackHitType AttackFireball;
	FAttackHitType AttackMultipleFireball;
	FAttackHitType AttackFirebreath;
	FAttackHitType AttackBite;
	FAttackHitType AttackLClaw;
	FAttackHitType AttackRClaw;
	FSoundType AttackBasicSound;
	FSoundType AttackBasicSound2;
	FSoundType AttackBasicSound3;
	FSoundType AttackBasicSound4;
	FSoundType AttackRSkillSound;


	UFUNCTION()
		void AnimNotify_DisableMovement();
	UFUNCTION()
		void AnimNotify_EnableMovement();
	UFUNCTION()
		void AnimNotify_DisableAboutDodge();
	UFUNCTION()
		void AnimNotify_EnableAboutDodge();
	UFUNCTION()
		void AnimNotify_DisableDuringHit();
	UFUNCTION()
		void AnimNotify_EnableDuringHit();
	UFUNCTION()
		void AnimNotify_DisableDuringAttack();
	UFUNCTION()
		void AnimNotify_EnableDuringAttack();
	UFUNCTION()
		void AnimNotify_DisableDuringDodge();
	UFUNCTION()
		void AnimNotify_EnableDuringDodge();
	UFUNCTION()
		void AnimNotify_BasicHit();
	UFUNCTION()
		void AnimNotify_QSkillHit();
	UFUNCTION()
		void AnimNotify_RskillHit();
	UFUNCTION()
		void AnimNotify_Fireball();
	UFUNCTION()
		void AnimNotify_MultipleFireball();
	UFUNCTION()
		void AnimNotify_Firebreath();
	UFUNCTION()
		void AnimNotify_AttackBite();
	UFUNCTION()
		void AnimNotify_AttackLClaw();
	UFUNCTION()
		void AnimNotify_AttackRClaw();
	UFUNCTION()
		void AnimNotify_AttackBasicSound();
	UFUNCTION()
		void AnimNotify_AttackBasicSound2();
	UFUNCTION()
		void AnimNotify_AttackBasicSound3();
	UFUNCTION()
		void AnimNotify_AttackBasicSound4();
	UFUNCTION()
		void AnimNotify_AttackRSkillSound();
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
public:
	void PlayHitReactMontage();
	void PlayDeathMontage();
	virtual void PlayAttackMontage();
	virtual void PlayRangeAttackMontage();

};

