	// Fill // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMonster.h"
#include "MonsterSkeleton.generated.h"

/**
 *
 */
UCLASS()
class SHOOTING_API AMonsterSkeleton : public ABaseMonster
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
		class UParticleSystem* TakeHitEffect;
public:
	AMonsterSkeleton();
protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	void Attack(AActor* Target);
	void Attack_Basic(float damage);

};