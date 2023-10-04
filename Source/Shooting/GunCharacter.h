// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GunCharacter.generated.h"

class AWeaponGun;
class UGunAnimInstance;
/**
 * 
 */
UCLASS()
class SHOOTING_API AGunCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float AimTargetLength;
protected:
	UPROPERTY(VisibleAnywhere)
		UGunAnimInstance* AnimInstance;
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
	
	FTimerHandle AimWaitHandle;
	bool IsAiming;

	void Attack();
	void AimingStart();
	void AimingEnd();

	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
