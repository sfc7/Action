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
};

USTRUCT()
struct FMonsterData : public FTableRowBase {
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int32 DragonMaxHp;
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int32 SkeletonMaxHp;

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int32 Hp;
};

UCLASS()
class SHOOTING_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	class UDataTable* CharacterDataTable;
	class UDataTable* MonsterDataTable;
protected:
		int32 KatanaMaxHp;
		int32 KatanaHp;
		int32 MagicMaxHp;
		int32 MagicHp;
public:
	UMyGameInstance();

	FOnEvent1_DeathCount OnEvent1_DeathCount;
	FOnEvent2_DeathCount OnEvent2_DeathCount;
public:
	virtual void Init() override;
public:
	FCharacterData* GetCharacterData();
	FMonsterData* GetMonsterData();
public:
	void KatanaSetHp(int32 _Hp);
	void MagicSetHp(int32 _Hp);
	int32 KatanaGetHp() { return KatanaHp; }
	int32 MagicGetHp() { return MagicHp; }
	int32 KatanaGetMaxHp() { return KatanaMaxHp; }
	int32 MagicGetMaxHp() { return MagicMaxHp; }


	void SetMonsterDeathCount();
	int32 MonsterDeathCount = 0;

	
};
