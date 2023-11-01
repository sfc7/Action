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
		int32 Hp;
};

UCLASS()
class SHOOTING_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	class UDataTable* CharacterDataTable;
	class UDataTable* MonsterDataTable;

	FOnEvent1_DeathCount OnEvent1_DeathCount;
	FOnEvent2_DeathCount OnEvent2_DeathCount;
protected:
		int32 KatanaMaxHp;
		int32 KatanaHp;
		int32 MagicMaxHp;
		int32 MagicHp;
		int32 GunMaxHp;
		int32 GunHp;

		int32 MonsterDeathCount;
public:
	UMyGameInstance();
public:
	virtual void Init() override;
public:
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





	
};
