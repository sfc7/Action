// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterUIWidget.generated.h"

class UMyGameInstance;
class UProgressBar;
/**
 * 
 */
UCLASS()
class SHOOTING_API UCharacterUIWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	class ABaseCharacter* player;

	UPROPERTY()
		UMyGameInstance* MyGameInstance;
	UPROPERTY(EditAnywhere, Category = Character)
		UTexture2D* KatanaIcon;
	UPROPERTY(EditAnywhere, Category = Character)
		UTexture2D* MagicIcon;
	UPROPERTY(EditAnywhere, Category = Character)
		UTexture2D* GunIcon;
	UPROPERTY(EditAnywhere, Category = Character)
		UTexture2D* KatanaQ;
	UPROPERTY(EditAnywhere, Category = Character)
		UTexture2D* MagicQ;
	UPROPERTY(EditAnywhere, Category = Character)
		UTexture2D* GunQ;
	UPROPERTY(EditAnywhere, Category = Character)
		UTexture2D* KatanaR;
	UPROPERTY(EditAnywhere, Category = Character)
		UTexture2D* MagicR;

	UPROPERTY(EditAnywhere, Category = Character)
		UTexture2D* GunR;
public:
	void BindHp(class UPlayerComponent* _Comp);
	void UpdateHp();
	void Bind_Q_Skill_CoolDown(class UPlayerComponent* _Comp);
	void Update_Q_Skill_CoolDown(class UPlayerComponent* _Comp);
	void SetKatanaCharacterInfo();
	void SetMagicCharacterInfo();
	void SetGunCharacterInfo();
	void UpdateSkillColldown(float DeltaTime, bool ShouldSkill, float SkillTime, float MaxTime, UProgressBar* CooldownWidget);
	void UpdateCharacterIconColldown(float KatanaCooldown, float MagicCooldown, float GunCooldown);

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

private:
	UPROPERTY()
		class UPlayerComponent* Comp;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HpBar;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* Q_Cooldown;
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* R_Cooldown;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Name;
	UPROPERTY(meta = (BindWidget))
		class UImage* Q;
	UPROPERTY(meta = (BindWidget))
		class UImage* R;
	UPROPERTY(meta = (BindWidget))
		class UImage* CharacterImage;
	UPROPERTY(meta = (BindWidget))
		class UImage* SwapKatana;
	UPROPERTY(meta = (BindWidget))
		class UImage* SwapMagic;
	UPROPERTY(meta = (BindWidget))
		class UImage* SwapGun;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Katana_Cooldown;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Magic_Cooldown;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Gun_Cooldown;




};
