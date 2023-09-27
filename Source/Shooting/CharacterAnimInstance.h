// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CreatureAnimInstance.h"
#include "CharacterAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FAttackComboCheck);
DECLARE_MULTICAST_DELEGATE(FAttackMontageEnd)

/**
 * 
 */
UCLASS()
class SHOOTING_API UCharacterAnimInstance : public UCreatureAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
public:
	UPROPERTY(Category = Movement, VisibleAnywhere, BlueprintReadWrite)
		FRotator ControllerRotation;
	UPROPERTY(Category = Movement, VisibleAnywhere, BlueprintReadWrite)
		FRotator ActorRotation;
	UPROPERTY(Category = Movement, VisibleAnywhere, BlueprintReadWrite)
		FRotator DeltaRotation;
public:
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadWrite)
		UAnimMontage* AttackMontage;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadWrite)
		UAnimMontage* Attack_Q_SkillMontage;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadWrite)
		UAnimMontage* Attack_R_SkillMontage;
	UPROPERTY(Category = Animation, EditAnywhere, BlueprintReadWrite)
		UAnimMontage* ForwardRollMontage;
public:
	FAttackComboCheck AttackComboCheck;
	FAttackMontageEnd AttackMontageEnd;

	UFUNCTION()
		void AnimNotify_NextAttack();
	UFUNCTION()
		void AnimNotify_AttackMontageEnd();

	FName GetAttackMontageSectionName(int Section);
	void JumpToAttackMontageSection(int Section);

public:
	void PlayAttackMontage();
	void PlayAttack_Q_SkillMontage();
	void PlayAttack_R_SkillMontage();
	void PlayForwardRollMontage();





};
