// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerComponent.h"
#include "MyGameInstance.h"

void UPlayerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UPlayerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerComponent::OnDamaged(float Damage)
{
	int32 temp = Hp - Damage;

	if (GameInstance)
	{
		if (GetOwner()->GetClass()->GetName() == "KatanaCharacter") {
			GameInstance->KatanaSetHp(temp);
			SetHp(temp);
		}
		else if (GetOwner()->GetClass()->GetName() == "MagicCharacter") {
			GameInstance->MagicSetHp(temp);
			SetHp(temp);
		}
		else if (GetOwner()->GetClass()->GetName() == "GunCharacter") {
			GameInstance->GunSetHp(temp);
			SetHp(temp);
		}
	}
}

void UPlayerComponent::SetInitialize()
{
	GameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance)
	{
		if (GetOwner()->GetClass()->GetName() == "KatanaCharacter") {
			MaxHp = GameInstance->KatanaGetMaxHp();
			Hp = GameInstance->KatanaGetHp();
		}
		else if (GetOwner()->GetClass()->GetName() == "MagicCharacter") {
			MaxHp = GameInstance->MagicGetMaxHp();
			Hp = GameInstance->MagicGetHp();
		}
		else if (GetOwner()->GetClass()->GetName() == "GunCharacter") {
			MaxHp = GameInstance->GunGetMaxHp();
			Hp = GameInstance->GunGetHp();
		}

		FTimerHandle waitHandle;
		GetWorld()->GetTimerManager().SetTimer(waitHandle, FTimerDelegate::CreateLambda([&]()
		{
			SetHp(Hp);

		}), 0.01f, false);
	}
}

void UPlayerComponent::SetHp(int32 _Hp)
{
	if (GameInstance)
	{
		if (GetOwner()->GetClass()->GetName() == "KatanaCharacter") {
			Hp = _Hp;
			GameInstance->KatanaSetHp(_Hp);
		}
		else if (GetOwner()->GetClass()->GetName() == "MagicCharacter") {
			Hp = _Hp;
			GameInstance->MagicSetHp(_Hp);
		}
	}

	Hp = _Hp;
	if (Hp <= 0) {
		Hp = 0;
		OnDeath.Broadcast();
		GameInstance->ResetGameState();
	}

	OnHpChanged.Broadcast();
}
