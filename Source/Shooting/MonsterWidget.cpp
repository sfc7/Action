// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterWidget.h"
#include "MonsterComponent.h"
#include "Components/ProgressBar.h"

void UMonsterWidget::BindHp(UMonsterComponent* _Comp)
{
	Comp = _Comp;
	Comp->OnHpChanged.AddUObject(this, &UMonsterWidget::UpdateHp);
}

void UMonsterWidget::UpdateHp()
{
	HpBar->SetPercent(Comp->GetHpRatio());
}
