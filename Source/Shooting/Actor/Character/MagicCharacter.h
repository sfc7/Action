// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "MagicCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API AMagicCharacter : public ABaseCharacter
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere)
		class UCharacterAnimInstance* AnimInstance;
	UPROPERTY(VisibleAnywhere)
		class UParticleSystem* MeteorPortal;
	UPROPERTY(VisibleAnywhere)
		class UParticleSystem* MeteorAura;
	UPROPERTY(VisibleAnywhere)
		class USoundWave* SpellSound;
	UPROPERTY(VisibleAnywhere)
		class USoundWave* AttackBasicSound2;
	UPROPERTY(VisibleAnywhere)
		class USoundWave* AttackBasicSound3;
	UPROPERTY(VisibleAnywhere)
		class USoundWave* AttackBasicSound4;
public:
	AMagicCharacter();
protected:
	virtual void BeginPlay() override;
public:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void Attack();
	void Attack_Skill_Q();
	void Attack_Skill_R();
	void Attack_Basic(float damage);
	void Attack_R();
	void Attack_Q();
	virtual void AttackStartComboState() override;
	virtual void AttackEndComboState() override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void Dodge();
	virtual void Death() override;
	void AttackBasicSound2Play();
	void AttackBasicSound3Play();
	void AttackBasicSound4Play();
	void SpellSoundPlay();
};
