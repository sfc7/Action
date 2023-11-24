// Fill out your copyright notice in the Description page of Project Settings.


#include "GunAimingHUD.h"
#include "Engine/Canvas.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Color.h"
#include "GunCharacter.h"

AGunAimingHUD::AGunAimingHUD()
{
	ConstructorHelpers::FObjectFinder<UTexture> DotAsset(TEXT("/Script/Engine.Texture2D'/Game/Shooting/BluePrint/UI/Dot.Dot'"));
	if (DotAsset.Succeeded()) {
		Dot = DotAsset.Object;
	}

	ConstructorHelpers::FObjectFinder<UTexture> LeftParenthesisAsset(TEXT("/Script/Engine.Texture2D'/Game/Shooting/BluePrint/UI/Left.Left'"));
	if (LeftParenthesisAsset.Succeeded()) {
		LeftParenthesis = LeftParenthesisAsset.Object;
	}

	ConstructorHelpers::FObjectFinder<UTexture> RightParenthesisAsset(TEXT("/Script/Engine.Texture2D'/Game/Shooting/BluePrint/UI/Right.Right'"));
	if (RightParenthesisAsset.Succeeded()) {
		RightParenthesis = RightParenthesisAsset.Object;
	}

	ConstructorHelpers::FObjectFinder<UFont> TextFontAsset(TEXT("/Script/Engine.Font'/Game/Shooting/BluePrint/UI/Font/digital-7__mono_italic__Font.digital-7__mono_italic__Font'"));
	if(TextFontAsset.Object)
	{
		TextFont = TextFontAsset.Object;
	}

}

void AGunAimingHUD::DrawHUD()
{
	Super::DrawHUD();

	AGunCharacter* PlayerCharacter = Cast<AGunCharacter>(GetOwningPlayerController()->GetPawn());
	if (PlayerCharacter && PlayerCharacter->IsZooming) {
			float DotSizeX = (Canvas->SizeX / 2.0f - 20.0f);
			float DotSizeY = (Canvas->SizeY / 2.0f - 25.0f);
			float OriginalLeftSizeX = DotSizeX - 120.0f;
			float OriginalRightSizeX = DotSizeX + 40.0f;
			float ParenthesisSizeY = DotSizeY - 50.0f;
			float Speed = -(PlayerCharacter->GetCharacterMovement()->Velocity.Size() / 15);
			FString Bullet = "";

			FLinearColor AimColor(255.0f, 255.0f, 255.0f, UKismetMathLibrary::MapRangeClamped(Speed, 0.0f, -53.0f, 0.8f, 0.05f));
			FLinearColor TextColor(230.0f / 255.0f, 255.0f / 230.0f, 230.0f / 255.0f, UKismetMathLibrary::MapRangeClamped(Speed, 0.0f, -53.0f, 1.0f, 0.5f));

			
			DrawTexture(Dot, DotSizeX, DotSizeY, 25.0f, 25.0f, 0.0f, 0.0f, 1.0f, 1.0f, AimColor, BLEND_Translucent, 1.0f);

			float LerpSpeed = 0.5f; 
			float TargetLeftX = PlayerCharacter->IsAimWaving ? (OriginalLeftSizeX - 50.f) : (OriginalLeftSizeX + Speed);
			float TargetRightX = PlayerCharacter->IsAimWaving ? (OriginalRightSizeX + 50.f) : (OriginalRightSizeX - Speed);
		
			float LeftParenthesisSizeX = PlayerCharacter->IsAimWaving ? FMath::Lerp(OriginalLeftSizeX, TargetLeftX, LerpSpeed) : FMath::Lerp(TargetLeftX, OriginalLeftSizeX, LerpSpeed);
			float RightParenthesisSizeX = PlayerCharacter->IsAimWaving ? FMath::Lerp(OriginalRightSizeX, TargetRightX, LerpSpeed) : FMath::Lerp(TargetRightX, OriginalRightSizeX, LerpSpeed);

			DrawTexture(LeftParenthesis, LeftParenthesisSizeX, ParenthesisSizeY, 100.0f, 125.0f, 0.0f, 0.0f, 1.0f, 1.0f, AimColor, BLEND_Translucent, 1.0f);
			DrawTexture(RightParenthesis, RightParenthesisSizeX, ParenthesisSizeY, 100.0f, 125.0f, 0.0f, 0.0f, 1.0f, 1.0f, AimColor, BLEND_Translucent, 1.0f);

			if (PlayerCharacter->IsAttacking_Q_Skill) {
				Bullet = "LASER";
				TextColor = FLinearColor(83.0f / 255.0f, 83.0f / 255.0f, 200.0f / 255.0f, 1.0f);
			}
			else {
				Bullet = "RIFLE";
				TextColor = FLinearColor(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 1.0f);
			}
			
			DrawText(Bullet, TextColor, LeftParenthesisSizeX + Speed - 100.0f, ParenthesisSizeY + 80.0f, TextFont, 4.0f);
			
	}
}