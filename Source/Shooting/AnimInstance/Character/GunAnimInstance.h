// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAnimInstance.h"
#include "GunAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FAbilityREnd);

class AGunCharacter;
/**
 * 
 */
UCLASS()
class SHOOTING_API UGunAnimInstance : public UCharacterAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsAiming;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float RotationYaw;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FRotator RSkillDeltaRotation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsAttackingRSkill;
	UPROPERTY(VisibleAnywhere)
		FRotator PrevRotation;
	UPROPERTY(VisibleAnywhere)
		FRotator CurRotation;
	UPROPERTY(VisibleAnywhere)
		float FinaldistanceCurve;
	UPROPERTY(VisibleAnywhere)
		float DistanceCurve;
	UPROPERTY(VisibleAnywhere)
		AGunCharacter* GunCreature;
public:
	FAbilityREnd AbilityREnd;

	UFUNCTION()
		void AnimNotify_Pause();
	UFUNCTION()
		void AnimNotify_AbilityREnd();

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
