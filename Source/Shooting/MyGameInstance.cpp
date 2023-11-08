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

void UMyGameInstance::Katana_Q_SkillCoolDownStart()
{
	SkillCoolDownStart(Katana_Q_SkillHandle, Katana_Q_Cooldown, 5.0f, Katana_Should_Q_Skill);
}

void UMyGameInstance::Katana_R_SkillCoolDownStart()
{
	SkillCoolDownStart(Katana_R_SkillHandle, Katana_R_Cooldown, 20.0f, Katana_Should_R_Skill);
}

void UMyGameInstance::Magic_Q_SkillCoolDownStart()
{
	SkillCoolDownStart(Magic_Q_SkillHandle, Magic_Q_Cooldown, 5.0f, Magic_Should_Q_Skill);
}

void UMyGameInstance::Magic_R_SkillCoolDownStart()
{
	SkillCoolDownStart(Magic_R_SkillHandle, Magic_R_Cooldown, 20.0f, Magic_Should_R_Skill);
}

void UMyGameInstance::Gun_Q_SkillCoolDownStart()
{
	SkillCoolDownStart(Gun_Q_SkillHandle, Gun_Q_Cooldown, 5.0f, Gun_Should_Q_Skill);
}

void UMyGameInstance::Gun_R_SkillCoolDownStart()
{
	SkillCoolDownStart(Gun_R_SkillHandle, Gun_R_Cooldown, 20.0f, Gun_Should_R_Skill);
}

void UMyGameInstance::Character_KatanaCoolDownStart()
{
	CharacterCoolDownStart(KatanaHandle, Character_Katana_Cooldown);
}

void UMyGameInstance::Character_MagicCoolDownStart()
{
	CharacterCoolDownStart(MagicHandle, Character_Magic_Cooldown);
}

void UMyGameInstance::Character_GunCoolDownStart()
{
	CharacterCoolDownStart(GunHandle, Character_Gun_Cooldown);
}

void UMyGameInstance::SkillCoolDownStart(FTimerHandle& SkillHandle, float& CharacterCooldown, float ApplyCooldown, bool& ShouldSkill)
{
	CharacterCooldown = ApplyCooldown;
	ShouldSkill = false;
	GetWorld()->GetTimerManager().SetTimer(SkillHandle, FTimerDelegate::CreateLambda([this, &SkillHandle, &CharacterCooldown, &ShouldSkill]() {
		CharacterCooldown -= 0.01f;
		if (CharacterCooldown <= 0.0f) {
			GetWorld()->GetTimerManager().ClearTimer(SkillHandle);
			CharacterCooldown = 0.0;
			ShouldSkill = true;
		}
	}), 0.01f, true);
}

void UMyGameInstance::CharacterCoolDownStart(FTimerHandle& CharacterHandle, float& CharacterCooldown)
{
	CharacterCooldown = 5.0f;
	GetWorld()->GetTimerManager().SetTimer(CharacterHandle, FTimerDelegate::CreateLambda([this, &CharacterHandle, &CharacterCooldown]() {
		CharacterCooldown -= 1.0f;
		if (CharacterCooldown <= 0.0f) {
			GetWorld()->GetTimerManager().ClearTimer(CharacterHandle);
			CharacterCooldown = 0.0;
		}
		}), 1.0f, true);
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

