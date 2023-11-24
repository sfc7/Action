// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "KatanaCharacter.generated.h"

class UKatanaAnimInstance;
/**
 *
 */
UCLASS()
class SHOOTING_API AKatanaCharacter : public ABaseCharacter
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere)
		UKatanaAnimInstance* AnimInstance;
	UPROPERTY(VisibleAnywhere)
		class UParticleSystem* HitEffect;
	UPROPERTY(VisibleAnywhere)
		class USoundWave* AttackBasicSound2;
	UPROPERTY(VisibleAnywhere)
		class USoundWave* AttackBasicSound3;



public:
	AKatanaCharacter();
protected:
	virtual void BeginPlay() override;
public:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
public:
	//Attack
	void Attack();
	void Attack_Skill_Q();
	void Attack_Skill_R();
	void Attack_Basic(float damage);
	virtual void AttackStartComboState() override;
	virtual void AttackEndComboState() override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void Dodge();
	virtual void Death() override;
	void AttackBasicSound2Play();
	void AttackBasicSound3Play();
};