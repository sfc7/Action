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
#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "BossFireBall.h"
#include "MultipleBossFireBall.h"


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
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> TakeEffectAsset(TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_hit2.P_ky_hit2'"));
	if (TakeEffectAsset.Succeeded()) {
		TakeHitEffect = TakeEffectAsset.Object;
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem> TeleportBodyEffectAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Shooting/BluePrint/Effect/P_Gideon_Burden_DoT.P_Gideon_Burden_DoT'"));
	if (TeleportBodyEffectAsset.Succeeded()) {
		TeleportBodyEffect = TeleportBodyEffectAsset.Object;
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem> TeleportTrailEffectAsset(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Meteor/FX/P_Gideon_Meteor_Trail.P_Gideon_Meteor_Trail'"));
	if (TeleportTrailEffectAsset.Succeeded()) {
		TeleportTrailEffect = TeleportTrailEffectAsset.Object;
	}

	static ConstructorHelpers::FClassFinder<AActor> Dagger(TEXT("/Script/Engine.Blueprint'/Game/Shooting/BluePrint/BP_Dagger.BP_Dagger_C'"));
	if (Dagger.Succeeded())
	{
		Weapon = Dagger.Class;
	}

	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	AIControllerClass = AMonster_Boss_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	SetArrowComponent();
}

void AMonsterBoss::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UMonsterBossAnimInstance>(GetMesh()->GetAnimInstance());
	if (IsValid(AnimInstance)) {
		AnimInstance->OnHit.AddUObject(this, &AMonsterBoss::ToggleDuringHit);
		AnimInstance->DuringAttack.AddUObject(this, &AMonsterBoss::ToggleDuringAttack);
		AnimInstance->AttackBasic.AddUObject(this, &AMonsterBoss::Attack_Basic, 10.0f);
		AnimInstance->AttackFireball.AddUObject(this, &AMonsterBoss::Spawn_Fireball);
		AnimInstance->AttackMultipleFireball.AddUObject(this, &AMonsterBoss::Spawn_MultipleFireball);
	}
}

void AMonsterBoss::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(Weapon))
	{
		FName socket = "Spine_Socket";
		FActorSpawnParameters SpawnParams;

		auto SpawnWeapon = GetWorld()->SpawnActor<AActor>(Weapon, SpawnParams);
		if (IsValid(SpawnWeapon)) {
			SpawnWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, socket);
		}
	}
}

void AMonsterBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void AMonsterBoss::Attack(AActor* Target)
{
	if (!IsAttacking && ShouldAttack) {
		AnimInstance->PlayAttackMontage();
	}
}

void AMonsterBoss::Attack_Basic(float damage)
{
	AttackBasicSoundPlay();
	FHitResult HitResult;
	float AttackRange = 100.f;
	float AttackRadius = 50.f;

	FVector Center = GetActorLocation();

	FVector Forward = Center + GetActorForwardVector() * AttackRange;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool Result = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		Center,
		Forward,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel7,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat();
	FColor DrawColor;

	if (Result && HitResult.GetActor()->GetClass()->GetSuperClass()->GetName() == TEXT("BaseCharacter"))
	{
		DrawColor = FColor::Green;

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TakeHitEffect, HitResult.ImpactPoint);

		AActor* HitActor = HitResult.GetActor();
		UGameplayStatics::ApplyDamage(HitActor, damage, GetController(), HitActor, NULL);
	}
	else {
		DrawColor = FColor::Red;
	}

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, Rotation, DrawColor, false, 3.f);
}

void AMonsterBoss::RangeAttack(AActor* Target)
{
	if (!IsAttacking && ShouldAttack) {
		TargetLocation = Target->GetActorLocation();
		SetAttackRotation(Target);
		AnimInstance->PlayRangeAttackMontage();
	}
}

void AMonsterBoss::Spawn_Fireball()
{
	FName SocketName = "Head";
	ABossFireBall* BossFireball = GetWorld()->SpawnActor<ABossFireBall>(ABossFireBall::StaticClass(), GetMesh()->GetSocketLocation(SocketName), GetActorRotation(), FActorSpawnParameters());
	if (BossFireball) {
		BossFireball->SetTargetLocation(TargetLocation);
	}
}

void AMonsterBoss::Spawn_MultipleFireball()
{
	FName SocketName = "hand_r_MagicSocket";
	AMultipleBossFireBall* MultipleBossFireball = GetWorld()->SpawnActor<AMultipleBossFireBall>(AMultipleBossFireBall::StaticClass(), GetMesh()->GetSocketLocation(SocketName), GetActorRotation(), FActorSpawnParameters());
	if (MultipleBossFireball) {
		MultipleBossFireball->SetTargetLocation(TargetLocation);
	}
}

void AMonsterBoss::SetArrowComponent()
{
	TArray<FVector> ArrowLocations {
		FVector(-85.0f, 0.0f, 120.0f),
		FVector(-25.0f, 50.0f, 105.0f),
		FVector(0.0f, 85.0f, 85.0f),
		FVector(0.0f, 50.0f, 50.0f),
		FVector(0.0f, -50.0f, 85.0f),
		FVector(0.0f, -80.0f, 105.0f),
		FVector(0.0f, -80.0f, 40.0f)
	};

	for (int32 i = 0; i < 7; i++) {
		FName ArrowName = FName(*FString::Printf(TEXT("Arrow%d"), i));
		auto Arrows = CreateDefaultSubobject<UArrowComponent>(ArrowName);
		Arrows->SetRelativeLocation(ArrowLocations[i]);
		Arrows->SetupAttachment(GetCapsuleComponent());
		ArrowComponents.Add(Arrows);
	}
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
