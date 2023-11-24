// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger_Stage1_Start.h"

ATrigger_Stage1_Start::ATrigger_Stage1_Start()
{
	static ConstructorHelpers::FClassFinder<AActor> DirectionArrowAsset(TEXT("/Script/Engine.Blueprint'/Game/Shooting/BluePrint/UI/Trigger/Arrow/ForwardDirectionArrow.ForwardDirectionArrow_C'"));
	if (DirectionArrowAsset.Succeeded()) {
		StaticDirectionArrow = DirectionArrowAsset.Class;
	}
}

void ATrigger_Stage1_Start::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ATrigger_Stage1_Start::Event);
	OnActorEndOverlap.AddDynamic(this, &ATrigger_Stage1_Start::EndEvent);
}

void ATrigger_Stage1_Start::Event(AActor* OverlapActor, AActor* OtherActor)
{

	if (!IsFlag) {
		if (OverlapActor && OtherActor != this) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			if (IsValid(StaticDirectionArrow)) {
				DirectionArrow = GetWorld()->SpawnActor<AActor>(
					StaticDirectionArrow,
					FVector(-4260.0f, 564.0f, 100.0f),
					FRotator(90.0f, 0.0f, 270.0f),
					SpawnParams
				);
			}
		}
		IsFlag = true;
	}
}

void ATrigger_Stage1_Start::EndEvent(AActor* OverlapActor, AActor* OtherActor)
{
	if (IsValid(DirectionArrow)) {
		DirectionArrow->Destroy();
	}

	Destroy();
}

