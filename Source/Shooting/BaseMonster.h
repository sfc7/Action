// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseMonster.generated.h"

DECLARE_MULTICAST_DELEGATE(FAttackMontageEnd)
/**
 * 
 */
UCLASS()
class SHOOTING_API ABaseMonster : public ACharacter
{
	GENERATED_BODY()
public:
	ABaseMonster();
public:
	UPROPERTY(VisibleAnywhere)
		class UCreatureAnimInstance* AnimInstance;
	UPROPERTY(EditAnywhere)
		class UWidgetComponent* HpBar;
	UPROPERTY(VisibleAnywhere)
		class UMonsterComponent* MonsterComponent;
	UPROPERTY(VisibleAnywhere)
		class USoundWave* AttackBasicSound;
	UPROPERTY(VisibleAnywhere)
		class USoundWave* HitSound;
	UPROPERTY(VisibleAnywhere)
		class USoundWave* DeathSound;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
public:
	bool ShouldAttack = true;
	bool ShouldMove = true;
	bool IsAttacking;
	bool IsDamaging;

public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	void ToggleDuringAttack(bool enable);
	void ToggleDuringHit(bool enable);

	void SetAttackRotation(AActor* Target);
	void Death();

	void AttackBasicSoundPlay();
	void HitSoundPlay();
	void DeathSoundPlay();
};
