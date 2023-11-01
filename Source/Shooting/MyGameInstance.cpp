// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DTCharacterData(TEXT("/Script/Engine.DataTable'/Game/Shooting/BluePrint/Data/DT_CharacterData.DT_CharacterData'"));
	if (DTCharacterData.Succeeded()) {
		CharacterDataTable = DTCharacterData.Object;
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> DTMonsterData(TEXT("/Script/Engine.DataTable'/Game/Shooting/BluePrint/Data/DT_MonsterData.DT_MonsterData'"));
	if (DTMonsterData.Succeeded()) {
		MonsterDataTable = DTMonsterData.Object;
	}

	ResetGameState();
}

void UMyGameInstance::Init()
{
	Super::Init();                                               
}

FCharacterData* UMyGameInstance::GetCharacterData()
{
	FName RowName = FName(TEXT("Basic"));

	return CharacterDataTable->FindRow<FCharacterData>(RowName, TEXT(""));
}

FMonsterData* UMyGameInstance::GetMonsterData()
{
	FName RowName = FName(TEXT("Basic"));

	return MonsterDataTable->FindRow<FMonsterData>(RowName, TEXT(""));
}

void UMyGameInstance::ResetGameState()
{
	KatanaMaxHp = GetCharacterData()->KatanaMaxHp;
	KatanaHp = KatanaMaxHp;
	MagicMaxHp = GetCharacterData()->MagicMaxHp;
	MagicHp = MagicMaxHp;
	GunMaxHp = GetCharacterData()->GunMaxHp;
	GunHp = GunMaxHp;
	MonsterDeathCount = 0;
}

void UMyGameInstance::KatanaSetHp(int32 _Hp)
{
	KatanaHp = _Hp;
}

void UMyGameInstance::MagicSetHp(int32 _Hp)
{
	MagicHp = _Hp;
}

void UMyGameInstance::GunSetHp(int32 _Hp)
{
	GunHp = _Hp;
}

void UMyGameInstance::SetMonsterDeathCount()
{
	MonsterDeathCount++;
	if (MonsterDeathCount == 3) {
		OnEvent1_DeathCount.Broadcast();
	}
	if (MonsterDeathCount == 5) {
		OnEvent2_DeathCount.Broadcast();
	}
}

