// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GunAimingHUD.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API AGunAimingHUD : public AHUD
{
	GENERATED_BODY()
public:
	AGunAimingHUD();
public:
	UPROPERTY()
		UTexture* Dot;
	UPROPERTY()
		UTexture* LeftParenthesis;
	UPROPERTY()
		UTexture* RightParenthesis;
	UPROPERTY(BlueprintReadOnly)
		UFont* TextFont;

	virtual void DrawHUD() override;
	
};
