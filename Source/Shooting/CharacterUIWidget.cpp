// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterUIWidget.h"
#include "PlayerComponent.h"
#include "Components/ProgressBar.h"
#include "BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "Components/ProgressBar.h"

void UCharacterUIWidget::BindHp(UPlayerComponent* _Comp)
{
	Comp = _Comp;
	Comp->OnHpChanged.AddUObject(this, &UCharacterUIWidget::UpdateHp);
}

void UCharacterUIWidget::UpdateHp()
{
	HpBar->SetPercent(Comp->GetHpRatio());

}

void UCharacterUIWidget::Bind_Q_Skill_CoolDown(UPlayerComponent* _Comp)
{
}

void UCharacterUIWidget::Update_Q_Skill_CoolDown(class UPlayerComponent* _Comp)
{
}

void UCharacterUIWidget::SetKatanaCharacterInfo()
{
	Name->SetText(FText::FromString("Katana"));
	Q->SetBrushFromTexture(KatanaQ);
	R->SetBrushFromTexture(KatanaR);
	CharacterImage->SetBrushFromTexture(KatanaIcon);
}

void UCharacterUIWidget::SetMagicCharacterInfo()
{
	Name->SetText(FText::FromString("Magic"));
	Q->SetBrushFromTexture(MagicQ);
	R->SetBrushFromTexture(MagicR);
	CharacterImage->SetBrushFromTexture(MagicIcon);
}

void UCharacterUIWidget::SetGunCharacterInfo()
{
	Name->SetText(FText::FromString("Gun"));
	Q->SetBrushFromTexture(GunQ);
	R->SetBrushFromTexture(GunR);
	CharacterImage->SetBrushFromTexture(GunIcon);
}

void UCharacterUIWidget::UpdateSkillColldown(float DeltaTime, bool ShouldSkill, float& SkillTime, float MaxTime, UProgressBar* CooldownWidget)
{
	if (ShouldSkill) {
		CooldownWidget->SetVisibility(ESlateVisibility::Hidden);
		CooldownWidget->SetPercent(1.0f);
		SkillTime = 0.0f;
	}
	else
	{
		SkillTime += DeltaTime;

		if (SkillTime >= MaxTime)
		{
			CooldownWidget->SetVisibility(ESlateVisibility::Hidden);
			CooldownWidget->SetPercent(1.0f);
			SkillTime = 0.0f;
		}
		else
		{
			CooldownWidget->SetVisibility(ESlateVisibility::Visible);
			float Percent = 1.0f - (SkillTime / MaxTime);
			CooldownWidget->SetPercent(Percent);
		}
	}

}

void UCharacterUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	KatanaIcon = LoadObject<UTexture2D>(NULL, TEXT("/Script/Engine.Texture2D'/Game/Shooting/BluePrint/UI/HealthBar/KatanaPicture.KatanaPicture'"));
	MagicIcon = LoadObject<UTexture2D>(NULL, TEXT("/Script/Engine.Texture2D'/Game/Shooting/BluePrint/UI/HealthBar/MagicPicture.MagicPicture'"));
	GunIcon = LoadObject<UTexture2D>(NULL, TEXT("/Script/Engine.Texture2D'/Game/Shooting/BluePrint/UI/HealthBar/GunPicture.GunPicture'"));

	KatanaQ = LoadObject<UTexture2D>(NULL, TEXT("/Script/Engine.Texture2D'/Game/Shooting/BluePrint/UI/Instance/Katana_SkillQImage.Katana_SkillQImage'"));
	KatanaR = LoadObject<UTexture2D>(NULL, TEXT("/Script/Engine.Texture2D'/Game/Shooting/BluePrint/UI/Instance/Katana_SkillRImage.Katana_SkillRImage'"));
	MagicQ = LoadObject<UTexture2D>(NULL, TEXT("/Script/Engine.Texture2D'/Game/Shooting/BluePrint/UI/Instance/Magic_SkillQImage.Magic_SkillQImage'"));
	MagicR = LoadObject<UTexture2D>(NULL, TEXT("/Script/Engine.Texture2D'/Game/Shooting/BluePrint/UI/Instance/Magic_SkillRimage.Magic_SkillRimage'"));
	GunQ = LoadObject<UTexture2D>(NULL, TEXT("/Script/Engine.Texture2D'/Game/Shooting/BluePrint/UI/Instance/Gun_SkillQImage.Gun_SkillQImage'"));
	GunR = LoadObject<UTexture2D>(NULL, TEXT("/Script/Engine.Texture2D'/Game/Shooting/BluePrint/UI/Instance/Gun_SkillRimage.Gun_SkillRimage'"));

}

void UCharacterUIWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	UpdateTimer += DeltaTime;

	if (GetOwningPlayerPawn()->GetClass()->GetSuperClass()->GetName() == "BaseCharacter")
	{
		player = Cast<ABaseCharacter>(GetOwningPlayerPawn());

		UpdateSkillColldown(DeltaTime, player->Should_Q_Skill, Qtime, 5.0f, Q_Cooldown);

		UpdateSkillColldown(DeltaTime, player->Should_R_Skill, Rtime, 20.0f, R_Cooldown);
	}

}




