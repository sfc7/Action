// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterComponent.h"
#include "MyGameInstance.h"

void UMonsterComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UMonsterComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMonsterComponent::SetInitialize()
{
	GameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance) {
		auto MonsterData = GameInstance->GetMonsterData();
		if (MonsterData) {
			if (GetOwner()->GetClass()->GetName() == "MonsterSkeleton") {
				MaxHp = MonsterData->SkeletonMaxHp;
				Hp = MaxHp;
			}
			else if (GetOwner()->GetClass()->GetName() == "MonsterDragon") {
				MaxHp = MonsterData->DragonMaxHp;
				Hp = MaxHp;
			}
			else if (GetOwner()->GetClass()->GetName() == "MonsterBoss") {
				MaxHp = MonsterData->BossMaxHp;
				Hp = MaxHp;
			}
		}
	}
}

void UMonsterComponent::SetHp(int32 _Hp)
{
	Hp = _Hp;
	if (Hp <= 0) {
		Hp = 0;
		OnDeath.Broadcast();
		if (GetOwner()->GetClass()->GetName() == "MonsterSkeleton") {
			GameInstance->SetMonsterDeathCount();
		}
	}

	OnHpChanged.Broadcast();
}

void UMonsterComponent::OnDamaged(float Damage)
{
	int32 temp = Hp - Damage;
	SetHp(temp);
}


