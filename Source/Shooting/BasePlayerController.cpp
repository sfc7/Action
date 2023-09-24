// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

ABasePlayerController::ABasePlayerController()
{

	static ConstructorHelpers::FClassFinder<UUserWidget> LoseText(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Shooting/BluePrint/UI/WBP_LoseText.WBP_LoseText_C'"));
	if (LoseText.Succeeded()) {
		ScreenLoseText = LoseText.Class;
	}

}

void ABasePlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);


	auto CreatedLoseText = CreateWidget(this, ScreenLoseText);
	if (CreatedLoseText != nullptr) {
		CreatedLoseText->AddToViewport();
	}

	FTimerHandle waitHandle;
	GetWorld()->GetTimerManager().SetTimer(waitHandle, this, &APlayerController::RestartLevel, 6.0f, false);
}
