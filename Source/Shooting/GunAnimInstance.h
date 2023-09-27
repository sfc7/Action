// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterAnimInstance.h"
#include "GunAnimInstance.generated.h"

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
public:
	UPROPERTY(VisibleAnywhere)
		AGunCharacter* GunCreature;
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
