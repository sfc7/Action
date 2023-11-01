// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMonster.h"
#include "MonsterBoss.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API AMonsterBoss : public ABaseMonster
{
	GENERATED_BODY()
public:
	AMonsterBoss();
public:
	FString Stance;
	
};
