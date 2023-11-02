// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBoss.h"
#include "MonsterBossAnimInstance.h"
#include "Monster_Boss_AIController.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundWave.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"


AMonsterBoss::AMonsterBoss()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Skeletal(TEXT("/Script/Engine.SkeletalMesh'/Game/Shooting/BluePrint/Mesh/Monster/BOSS/Ch40_nonPBR__2_.Ch40_nonPBR__2_'"));
	if (Skeletal.Succeeded()) {
		GetMesh()->SetSkeletalMesh(Skeletal.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> Anim(TEXT("/Script/Engine.AnimBlueprint'/Game/Shooting/BluePrint/ABP_Boss.ABP_Boss_C'"));
	if (Anim.Succeeded()) {
		GetMesh()->SetAnimClass(Anim.Class);
	}
	
	ConstructorHelpers::FObjectFinder<UParticleSystem> TeleportBodyEffectAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Shooting/BluePrint/Effect/P_Gideon_Burden_DoT.P_Gideon_Burden_DoT'"));
	if (TeleportBodyEffectAsset.Succeeded()) {
		TeleportBodyEffect = TeleportBodyEffectAsset.Object;
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem> TeleportTrailEffectAsset(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Meteor/FX/P_Gideon_Meteor_Trail.P_Gideon_Meteor_Trail'"));
	if (TeleportTrailEffectAsset.Succeeded()) {
		TeleportTrailEffect = TeleportTrailEffectAsset.Object;
	}

	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	AIControllerClass = AMonster_Boss_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AMonsterBoss::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMonsterBoss::BeginPlay()
{
	Super::BeginPlay();
}

void AMonsterBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void AMonsterBoss::Attack(AActor* Target)
{
}

void AMonsterBoss::Teleport(FVector _Location)
{
	auto Movement = GetCharacterMovement();
	Movement->SetMovementMode(EMovementMode::MOVE_Flying);
	Movement->MaxFlySpeed = 5000.0f;
	Movement->MaxAcceleration = 99999.0f;
	GetMesh()->SetVisibility(false, true);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));

	TeleportBodyComponent = UGameplayStatics::SpawnEmitterAttached(TeleportBodyEffect, GetMesh(), TEXT("Spine1"));
	TeleportTrailComponent = UGameplayStatics::SpawnEmitterAttached(TeleportTrailEffect, GetMesh(), TEXT("Spine1"));

	AIController->TeleportRequestID = AIController->MoveToLocation(_Location);
	AnimInstance->StopAllMontages(10);
}

void AMonsterBoss::TeleportEnd()
{
	
	auto Movement = GetCharacterMovement();
	Movement->StopMovementImmediately();
	Movement->SetMovementMode(EMovementMode::MOVE_Walking);
	Movement->MaxAcceleration = 1500.0f;
	GetMesh()->SetVisibility(true, true);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));
	
	FTimerHandle Waithandle;
	GetWorld()->GetTimerManager().SetTimer(Waithandle, FTimerDelegate::CreateLambda([&]() {
		if (TeleportBodyComponent) {
			TeleportBodyComponent->DestroyComponent();
		}
		if (TeleportTrailComponent) {
			TeleportTrailComponent->DestroyComponent();	
		}
	}), 0.5f, false);
	
}
