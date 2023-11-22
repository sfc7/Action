// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "MyGameInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnEvent1_DeathCount);
DECLARE_MULTICAST_DELEGATE(FOnEvent2_DeathCount);
/**
 * 
 */
USTRUCT()
struct FCharacterData : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int32 KatanaMaxHp;
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int32 KatanaHp;
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int32 MagicMaxHp;
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int32 MagicHp;
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int32 GunMaxHp;
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int32 GunHp;
};

USTRUCT()
struct FMonsterData : public FTableRowBase {
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int32 DragonMaxHp;
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int32 SkeletonMaxHp;
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int32 BossMaxHp;

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int32 Hp;
};

UCLASS()
class SHOOTING_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UMyGameInstance();
public:
	class UDataTable* CharacterDataTable;
	class UDataTable* MonsterDataTable;

	FOnEvent1_DeathCount OnEvent1_DeathCount;
	FOnEvent2_DeathCount OnEvent2_DeathCount;

	bool Katana_Should_Q_Skill = true;
	bool Katana_Should_R_Skill = true;
	bool Magic_Should_Q_Skill = true;
	bool Magic_Should_R_Skill = true;
	bool Gun_Should_Q_Skill = true;
	bool Gun_Should_R_Skill = true;

	float Katana_Q_Cooldown = 0.0f;
	float Katana_R_Cooldown = 0.0f;
	float Magic_Q_Cooldown = 0.0f;
	float Magic_R_Cooldown = 0.0f;
	float Gun_Q_Cooldown = 0.0f;
	float Gun_R_Cooldown = 0.0f;

	FTimerHandle Katana_Q_SkillHandle;
	FTimerHandle Katana_R_SkillHandle;
	FTimerHandle Magic_Q_SkillHandle;
	FTimerHandle Magic_R_SkillHandle;
	FTimerHandle Gun_Q_SkillHandle;
	FTimerHandle Gun_R_SkillHandle;

	float Character_Katana_Cooldown = 0.0f;
	float Character_Magic_Cooldown = 0.0f;
	float Character_Gun_Cooldown = 0.0f;

	FTimerHandle KatanaHandle;
	FTimerHandle MagicHandle;
	FTimerHandle GunHandle;
protected:
		int32 KatanaMaxHp;
		int32 KatanaHp;
		int32 MagicMaxHp;
		int32 MagicHp;
		int32 GunMaxHp;
		int32 GunHp;

		int32 MonsterDeathCount;
public:
	virtual void Init() override;

	void SetMonsterDeathCount();
	void ResetGameState();
	void KatanaSetHp(int32 _Hp);
	void MagicSetHp(int32 _Hp);
	void GunSetHp(int32 _Hp);

	FCharacterData* GetCharacterData();
	FMonsterData* GetMonsterData();
	int32 KatanaGetHp() { return KatanaHp; }
	int32 MagicGetHp() { return MagicHp; }
	int32 GunGetHp() { return GunHp; }
	int32 KatanaGetMaxHp() { return KatanaMaxHp; }
	int32 MagicGetMaxHp() { return MagicMaxHp; }
	int32 GunGetMaxHp() { return GunMaxHp; }


	void Katana_Q_SkillCoolDownStart();
	void Katana_R_SkillCoolDownStart();
	void Magic_Q_SkillCoolDownStart();
	void Magic_R_SkillCoolDownStart();
	void Gun_Q_SkillCoolDownStart();
	void Gun_R_SkillCoolDownStart();
	void Character_KatanaCoolDownStart();
	void Character_MagicCoolDownStart();
	void Character_GunCoolDownStart();

	void SkillCoolDownStart(FTimerHandle& SkillHandle, float& CharacterCooldown, float ApplyCooldown, bool& ShouldSkill);
	void CharacterCoolDownStart(FTimerHandle& CharacterHandle, float& CharacterCooldoWwn);
	
};
