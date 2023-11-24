// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API UMonsterWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void BindHp(class UMonsterComponent* _Comp);
	void UpdateHp();

private:
	UPROPERTY()
		class UMonsterComponent* Comp;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HpBar;
};
