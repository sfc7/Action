// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger_Stage1_Event1.h"
#include "BaseCharacter.h"
#include "MonsterSkeleton.h"
#include "RespawnLocation_Stage1_Event1.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"

ATrigger_Stage1_Event1::ATrigger_Stage1_Event1()
{
	static ConstructorHelpers::FClassFinder<AActor> NoSignWall_1Asset(TEXT("/Script/Engine.Blueprint'/Game/Shooting/BluePrint/UI/Trigger/Wall/BP_NoSignWall1.BP_NoSignWall1_C'"));
	if (NoSignWall_1Asset.Succeeded()) {
		StaticNoSignWall_1 = NoSignWall_1Asset.Class;
	}

	static ConstructorHelpers::FClassFinder<AActor> NoSignWall_2Asset(TEXT("/Script/Engine.Blueprint'/Game/Shooting/BluePrint/UI/Trigger/Wall/BP_NoSignWall2.BP_NoSignWall2_C'"));
	if (NoSignWall_2Asset.Succeeded()) {
		StaticNoSignWall_2 = NoSignWall_2Asset.Class;
	}

	static ConstructorHelpers::FClassFinder<AActor> DirectionArrowAsset1(TEXT("/Script/Engine.Blueprint'/Game/Shooting/BluePrint/UI/Trigger/Arrow/DirectionArrow.DirectionArrow_C'"));
	if (DirectionArrowAsset1.Succeeded()) {
		StaticDirectionArrow1 = DirectionArrowAsset1.Class;
	}

	static ConstructorHelpers::FClassFinder<AActor> DirectionArrowAsset2(TEXT("/Script/Engine.Blueprint'/Game/Shooting/BluePrint/UI/Trigger/Arrow/BelowdDirectionArrow.BelowdDirectionArrow_C'"));
	if (DirectionArrowAsset2.Succeeded()) {
		StaticDirectionArrow2 = DirectionArrowAsset2.Class;
	}
}

void ATrigger_Stage1_Event1::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ATrigger_Stage1_Event1::Event);
	auto GameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance) {
		GameInstance->OnEvent1_DeathCount.AddUObject(this, &ATrigger_Stage1_Event1::OnMonsterDeath);
	}
}

void ATrigger_Stage1_Event1::OnMonsterDeath()
{
	if (IsValid(NoSignWall_1)) {
		NoSignWall_1->Destroy();
	}
	if (IsValid(StaticDirectionArrow1)) {
		DirectionArrow1 = GetWorld()->SpawnActor<AActor>(
			StaticDirectionArrow1,
			FVector(-2583.0f, -153.0f, 542.0f),
			FRotator(0.0f, 0.0f, 0.0f)
		);
	}
	if (IsValid(StaticDirectionArrow2)) {
		DirectionArrow2 = GetWorld()->SpawnActor<AActor>(
			StaticDirectionArrow2,
			FVector(-2503.0f, -2427.0f, 652.0f),
			FRotator(0.0f, -90.0f, 270.0f)
		);
	}
}

void ATrigger_Stage1_Event1::Event(AActor* OverlapActor, AActor* OtherActor)
{
	TArray<AMonsterSkeleton*> MonsterArray;

	if (!IsFlag) {
		if (OverlapActor && OtherActor != this) {
			auto Base = Cast<ABaseCharacter>(OtherActor);
			if (Base) {
				TArray<AActor*> FoundArray;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARespawnLocation_Stage1_Event1::StaticClass(), FoundArray);

				for (auto FoundActor : FoundArray) {
					auto spawnSkeleton = GetWorld()->SpawnActor<AMonsterSkeleton>(
						AMonsterSkeleton::StaticClass(),
						FoundActor->GetActorLocation(),
						FoundActor->GetActorRotation()
					);
					MonsterArray.Add(spawnSkeleton);
				}

				if (IsValid(StaticNoSignWall_1)) {
					NoSignWall_1 = GetWorld()->SpawnActor<AActor>(
						StaticNoSignWall_1,
						FVector(-2788.0f, -698.0f, 181.0f),
						FRotator(0.0f, 0.0f, 0.0f)
					);
				}


				if (IsValid(StaticNoSignWall_2)) {
					NoSignWall_2 = GetWorld()->SpawnActor<AActor>(
						StaticNoSignWall_2,
						FVector(-3027.0f, -534.0f, 180.0f),
						FRotator(0.0f, 0.0f, 0.0f)
					);
				}
			}
			IsFlag = true;
		}
	}




}


