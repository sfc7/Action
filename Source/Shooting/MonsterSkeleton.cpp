// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSkeleton.h"
#include "Monster_Skeleton_AIController.h"
#include "MonsterSkeletonAnimInstance.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundWave.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "MonsterWidget.h"

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

	static ConstructorHelpers::FObjectFinder<USoundWave> AttackBasicSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Monster/Skeleton/SW_Skeleton_AttackBasic.SW_Skeleton_AttackBasic'"));
	if (AttackBasicSoundAsset.Succeeded()) {
		AttackBasicSound = AttackBasicSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> HitSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Monster/Skeleton/SW_Skeleton_Hit.SW_Skeleton_Hit'"));
	if (HitSoundAsset.Succeeded()) {
		HitSound = HitSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> DeathSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Monster/Skeleton/SW_Skeleton_Death.SW_Skeleton_Death'"));
	if (DeathSoundAsset.Succeeded()) {
		DeathSound = DeathSoundAsset.Object;
	}
	
	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UMonsterWidget> MW(TEXT("/Script/UserWidget'/Game/Shooting/BluePrint/UI/WBP_MonsterHpBar.WBP_MonsterHpBar_C'"));


	if (MW.Succeeded()) {
		HpBar->SetWidgetClass(MW.Class);
		HpBar->SetDrawSize(FVector2D(300.f, 30.f));
		HpBar->SetRelativeLocation(FVector(0.f, 0.f, 180.f));
	}

	GetCharacterMovement()->MaxWalkSpeed = 100.0f;
	
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
		AnimInstance->AttackBasic.AddUObject(this, &AMonsterSkeleton::Attack_Basic, 10.0f);
	}
}

void AMonsterSkeleton::BeginPlay()
{
	Super::BeginPlay();

	auto HpWidget = Cast<UMonsterWidget>(HpBar->GetUserWidgetObject());
	if (HpWidget) {
		HpWidget->BindHp(MonsterComponent);
	}
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

	/*DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, Rotation, DrawColor, false, 3.f);*/
}
