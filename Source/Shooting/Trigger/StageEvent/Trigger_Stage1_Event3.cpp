// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger_Stage1_Event3.h"
#include "Kismet/GameplayStatics.h"


ATrigger_Stage1_Event3::ATrigger_Stage1_Event3()
{

}

void ATrigger_Stage1_Event3::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ATrigger_Stage1_Event3::Event);
}

void ATrigger_Stage1_Event3::Event(AActor* OverlapActor, AActor* OtherActor)
{
	UGameplayStatics::OpenLevel(this, "Stage2");
	
}
