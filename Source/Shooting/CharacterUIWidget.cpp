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

void UCharacterUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	KatanaIcon = LoadObject<UTexture2D>(NULL, TEXT("/Script/Engine.Texture2D'/Game/Shooting/BluePrint/UI/HealthBar/KatanaPicture.KatanaPicture'"));
	MagicIcon = LoadObject<UTexture2D>(NULL, TEXT("/Script/Engine.Texture2D'/Game/Shooting/BluePrint/UI/HealthBar/MagicPicture.MagicPicture'"));
	KatanaQ = LoadObject<UTexture2D>(NULL, TEXT("/Script/Engine.Texture2D'/Game/Shooting/BluePrint/UI/Instance/Katana_SkillQImage.Katana_SkillQImage'"));
	KatanaR = LoadObject<UTexture2D>(NULL, TEXT("/Script/Engine.Texture2D'/Game/Shooting/BluePrint/UI/Instance/Katana_SkillRImage.Katana_SkillRImage'"));
	MagicQ = LoadObject<UTexture2D>(NULL, TEXT("/Script/Engine.Texture2D'/Game/Shooting/BluePrint/UI/Instance/Magic_SkillQImage.Magic_SkillQImage'"));
	MagicR = LoadObject<UTexture2D>(NULL, TEXT("/Script/Engine.Texture2D'/Game/Shooting/BluePrint/UI/Instance/Magic_SkillRimage.Magic_SkillRimage'"));

}

void UCharacterUIWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	UpdateTimer += DeltaTime;

	if (UpdateTimer >= UpdateInterval) {
		if (GetOwningPlayerPawn()->GetClass()->GetSuperClass()->GetName() == "BaseCharacter") {
			player = Cast<ABaseCharacter>(GetOwningPlayerPawn());
			bool ShouldQ = player->Should_Q_Skill;
			bool ShouldR = player->Should_R_Skill;

			if (ShouldQ) {
				Q_Cooldown->SetVisibility(ESlateVisibility::Hidden);
				Q_Cooldown->SetPercent(1.0f);
				Qtime = 0.0f;
			}
			else {
				Q_Cooldown->SetVisibility(ESlateVisibility::Visible);
				Q_Cooldown->SetPercent(0.0f);
				if (Qtime < 5.0f) {
					Qtime += 0.1f;
					Q_Cooldown->SetPercent(1.0 - UKismetMathLibrary::NormalizeToRange((double)Qtime, 0.0f, 5.0f));
					if (Qtime > 5.0f) {
						Q_Cooldown->SetVisibility(ESlateVisibility::Hidden);
						Q_Cooldown->SetPercent(1.0f);
						Qtime = 0.0f;
					}
				}
			}
			if (ShouldR) {
				R_Cooldown->SetVisibility(ESlateVisibility::Hidden);
				R_Cooldown->SetPercent(1.0f);
				Rtime = 0.0f;
			}

			else {
				R_Cooldown->SetVisibility(ESlateVisibility::Visible);
				R_Cooldown->SetPercent(0.0f);
				if (Rtime < 20.0f) {
					Rtime += 0.1f;
					R_Cooldown->SetPercent(1.0 - UKismetMathLibrary::NormalizeToRange((double)Rtime, 0.0f, 20.0f));
					if (Rtime > 20.0f) {
						R_Cooldown->SetVisibility(ESlateVisibility::Hidden);
						R_Cooldown->SetPercent(1.0f);
						Rtime = 0.0f;
					}
				}
			}

			UpdateTimer = 0.0f;
		}
	}







}




