// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger_Stage1_Event2.h"
#include "BaseCharacter.h"
#include "MonsterSkeleton.h"
#include "RespawnLocation_Stage1_Event2.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"


ATrigger_Stage1_Event2::ATrigger_Stage1_Event2()
{
	static ConstructorHelpers::FClassFinder<AActor> NoSignWall_3Asset(TEXT("/Script/Engine.Blueprint'/Game/Shooting/BluePrint/UI/Trigger/Wall/BP_NoSignWall3.BP_NoSignWall3_C'"));
	if (NoSignWall_3Asset.Succeeded()) {
		StaticNoSignWall_3 = NoSignWall_3Asset.Class;
	}

	static ConstructorHelpers::FClassFinder<AActor> DirectionArrowAsset(TEXT("/Script/Engine.Blueprint'/Game/Shooting/BluePrint/UI/Trigger/Arrow/ForwardDirectionArrow.ForwardDirectionArrow_C'"));
	if (DirectionArrowAsset.Succeeded()) {
		StaticDirectionArrow1 = DirectionArrowAsset.Class;
	}
}

void ATrigger_Stage1_Event2::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ATrigger_Stage1_Event2::Event);
	auto GameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance) {
		GameInstance->OnEvent2_DeathCount.AddUObject(this, &ATrigger_Stage1_Event2::OnMonsterDeath);
	}
}

void ATrigger_Stage1_Event2::Event(AActor* OverlapActor, AActor* OtherActor)
{
	TArray<AMonsterSkeleton*> MonsterArray;

	if (!IsFlag) {
		if (OverlapActor && OtherActor != this) {
			auto Base = Cast<ABaseCharacter>(OtherActor);
			if (Base) {
				TArray<AActor*> FoundArray;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARespawnLocation_Stage1_Event2::StaticClass(), FoundArray);

				for (auto FoundActor : FoundArray) {
					auto spawnSkeleton = GetWorld()->SpawnActor<AMonsterSkeleton>(
						AMonsterSkeleton::StaticClass(),
						FoundActor->GetActorLocation(),
						FoundActor->GetActorRotation()
					);
					MonsterArray.Add(spawnSkeleton);
				}

				if (IsValid(StaticNoSignWall_3)) {
					NoSignWall_3 = GetWorld()->SpawnActor<AActor>(
						StaticNoSignWall_3,
						FVector(-2341.0f, -4173.0f, 322.0f),
						FRotator(0.0f, 0.0f, 0.0f)
					);
				}
			}
		}
		IsFlag = true;
	}

}

void ATrigger_Stage1_Event2::OnMonsterDeath()
{
	if (IsValid(NoSignWall_3)) {
		NoSignWall_3->Destroy();
	}

	if (IsValid(StaticDirectionArrow1)) {
		DirectionArrow1 = GetWorld()->SpawnActor<AActor>(
			StaticDirectionArrow1,
			FVector(-2654.0f, -3940.0f, 650.0f),
			FRotator(0.0f, 90.0f, 0.0f)
		);
	}
}
