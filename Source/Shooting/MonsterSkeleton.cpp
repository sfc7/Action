// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSkeleton.h"
#include "Monster_Skeleton_AIController.h"
#include "MonsterSkeletonAnimInstance.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundWave.h"

AMonsterSkeleton::AMonsterSkeleton()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Skeletal(TEXT("/Script/Engine.SkeletalMesh'/Game/Shooting/BluePrint/Mesh/Monster/Skeleton/SkeletonWarriorUnrealVer.SkeletonWarriorUnrealVer'"));
	if (Skeletal.Succeeded()) {
		GetMesh()->SetSkeletalMesh(Skeletal.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> Anim(TEXT("/Script/Engine.AnimBlueprint'/Game/Shooting/BluePrint/ABP_Skeleton.ABP_Skeleton_C'"));
	if (Anim.Succeeded()) {
		GetMesh()->SetAnimClass(Anim.Class);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> TakeEffectAsset(TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_hit2.P_ky_hit2'"));
	if (TakeEffectAsset.Succeeded()) {
		TakeHitEffect = TakeEffectAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> AttackBasicSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Monster/Skeleton/SV_Skeleton_AttackBasic.SV_Skeleton_AttackBasic'"));
	if (AttackBasicSoundAsset.Succeeded()) {
		AttackBasicSound = AttackBasicSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> HitSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Monster/Skeleton/SV_Skeleton_Hit.SV_Skeleton_Hit'"));
	if (HitSoundAsset.Succeeded()) {
		HitSound = HitSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> DeathSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Monster/Skeleton/SV_Skeleton_Death.SV_Skeleton_Death'"));
	if (DeathSoundAsset.Succeeded()) {
		DeathSound = DeathSoundAsset.Object;
	}


	AIControllerClass = AMonster_Skeleton_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AMonsterSkeleton::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UMonsterSkeletonAnimInstance>(GetMesh()->GetAnimInstance());
	if (IsValid(AnimInstance)) {
		AnimInstance->OnHit.AddUObject(this, &AMonsterSkeleton::ToggleDuringHit);
		AnimInstance->DuringAttack.AddUObject(this, &AMonsterSkeleton::ToggleDuringAttack);
		AnimInstance->AttackBasic.AddUObject(this, &AMonsterSkeleton::Attack_Basic, 50.0f);
	}
}

void AMonsterSkeleton::BeginPlay()
{
	Super::BeginPlay();
}

void AMonsterSkeleton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AMonsterSkeleton::Attack(AActor *target)
{
	if (!IsAttacking && ShouldAttack) {
		AnimInstance->PlayAttackMontage();
	}
}

void AMonsterSkeleton::Attack_Basic(float damage)
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
