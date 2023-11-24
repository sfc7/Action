// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMonster.h"
#include "MonsterDragon.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API AMonsterDragon : public ABaseMonster
{
	GENERATED_BODY()
public:
	AMonsterDragon();

	UPROPERTY(VisibleAnywhere)
		class USoundWave* FireBallSound;
	UPROPERTY(VisibleAnywhere)
		class USoundWave* FireBreathSound;
protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	FVector TargetLocation;
	float WalkSpeed = 300.0f;
	
	void Attack(AActor* Target);
	void Attack_Bite(float damage);
	void Attack_LClaw(float damage);
	void Attack_RClaw(float damage);
	void RangeAttack(AActor* Target);
	void Spawn_Fireball();
	void Spawn_Firebreath();
	void AttackChannel(FName _SocketName, float _Damage);

	void FireBreathSoundPlay();
	void FireBallSoundPlay();
};
