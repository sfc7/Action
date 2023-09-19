// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GunCharacter.generated.h"

class AWeaponGun;
/**
 * 
 */
UCLASS()
class SHOOTING_API AGunCharacter : public ABaseCharacter
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere)
		class UCharacterAnimInstance* AnimInstance;
	UPROPERTY()
		AWeaponGun* WeaponGun;
public:
	AGunCharacter();
protected:
	virtual void BeginPlay() override;
public:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	void Attack();
};
