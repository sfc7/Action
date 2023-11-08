// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger_Stage2_Event1.h"
#include "MonsterDragon.h"
#include "MonsterBoss.h"
#include "BaseCharacter.h"
#include "RespawnLocation_Stage2_Event1.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"

ATrigger_Stage2_Event1::ATrigger_Stage2_Event1()
{
}

void ATrigger_Stage2_Event1::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ATrigger_Stage2_Event1::Event);
}

void ATrigger_Stage2_Event1::Event(AActor* OverlapActor, AActor* OtherActor)
{
	if (!IsFlag) {
		if (OverlapActor && OtherActor != this) {
			auto Base = Cast<ABaseCharacter>(OtherActor);
			if (Base) {
				TArray<AActor*> FoundArray;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARespawnLocation_Stage2_Event1::StaticClass(), FoundArray);

				for (auto FoundActor : FoundArray) {
					auto spawnDragon = GetWorld()->SpawnActor<AMonsterDragon>(
						AMonsterDragon::StaticClass(),
						FoundActor->GetActorLocation(),
						FoundActor->GetActorRotation()
					);
				}
			}
		}
		IsFlag = true;
	}
}
