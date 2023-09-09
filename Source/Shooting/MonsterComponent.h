// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CreatureComponent.h"
#include "MonsterComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API UMonsterComponent : public UCreatureComponent
{
	GENERATED_BODY()
protected:
	// Called when the game starts
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
public:
	void SetInitialize() override;
	void SetHp(int32 _Hp) override;
	void OnDamaged(float Damage);
};
