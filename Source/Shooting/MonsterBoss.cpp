// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBoss.h"
#include "MonsterBossAnimInstance.h"
#include "Monster_Boss_AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/WidgetComponent.h"
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
#include "Hammer.h"
#include "MonsterWidget.h"

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

	ConstructorHelpers::FObjectFinder<UParticleSystem> DashAttackBodyEffectAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Shooting/BluePrint/Effect/P_Mudang_AttackWolves_Cast.P_Mudang_AttackWolves_Cast'"));
	if (DashAttackBodyEffectAsset.Succeeded()) {
		DashAttackBodyEffect = DashAttackBodyEffectAsset.Object;
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem> DashAttackTrailEffectAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Shooting/BluePrint/Effect/P_Attack_Wolves_Trails.P_Attack_Wolves_Trails'"));
	if (DashAttackTrailEffectAsset.Succeeded()) {
		DashAttackTrailEffect = DashAttackTrailEffectAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> GatePortalEffectAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Shooting/BluePrint/Effect/P_Portal_Entrance.P_Portal_Entrance'"));
	if (GatePortalEffectAsset.Succeeded()) {
		GatePortalEffect = GatePortalEffectAsset.Object;
	}

	static ConstructorHelpers::FClassFinder<AActor> Dagger(TEXT("/Script/Engine.Blueprint'/Game/Shooting/BluePrint/BP_Dagger.BP_Dagger_C'"));
	if (Dagger.Succeeded())
	{
		Weapon = Dagger.Class;
	}

	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	AIControllerClass = AMonster_Boss_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FClassFinder<UMonsterWidget> MonsterWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Shooting/BluePrint/UI/WBP_MonsterBossHpBar.WBP_MonsterBossHpBar_C'"));

	if (MonsterWidget.Succeeded()) {
		MUI = CreateWidget<UMonsterWidget>(GetWorld(), MonsterWidget.Class);
		if (IsValid(MUI)) {
			MUI->AddToViewport();
		}
	}

	TArray<FVector> ArrowLocations {
		FVector(-85.0f, -90.0f, 175.0f),
		FVector(-25.0f, 0.0f, 170.0f),
		FVector(0.0f, 170.0f, 85.0f),
		FVector(20.0f, 80.0f, 30.0f),
		FVector(0.0f, 75.0f, 115.0f),
		FVector(0.0f, -155.0f, 105.0f),
		FVector(0.0f, -130.0f, 0.0f)
	};

	for (int32 i = 0; i < 7; i++) {
		FName ArrowName = FName(*FString::Printf(TEXT("Arrow%d"), i));
		auto Arrows = CreateDefaultSubobject<UArrowComponent>(ArrowName);
		Arrows->SetRelativeLocation(ArrowLocations[i]);
		Arrows->SetupAttachment(GetCapsuleComponent());
		ArrowComponents.Add(Arrows);
	}
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

	if (MUI) {
		MUI->BindHp(MonsterComponent);
	}

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMonsterBoss::OnOverlapBegin);
}

void AMonsterBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMonsterBoss::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{
		UGameplayStatics::ApplyDamage(OtherActor, 20.0f, nullptr, this, NULL);
	}
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

void AMonsterBoss::Spawn_GateofBabylon()
{
	for (auto ArrowLocationComponent : ArrowComponents) {
		FVector Location = ArrowLocationComponent->GetComponentLocation();
		FRotator Rotation = ArrowLocationComponent->GetComponentRotation();

		UParticleSystemComponent* Portal = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GatePortalEffect, Location, Rotation);
		Portal->CustomTimeDilation = 2.0f;

		FTimerHandle TimerHandle;
		float DestroyDelay = 4.0f;

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [Portal]() {
			if (Portal)
			{
				Portal->DeactivateSystem();
				Portal->DestroyComponent();
			}
			}, DestroyDelay, false);

		for (int32 i = 0; i < 6; i++) {
			FTimerHandle WaitHandle;
			float RandomDelay = FMath::RandRange(0.5f, 1.0f) + 0.5f * i;
			FVector HammerLocation = Location;
			FRotator HammerRotation = Rotation;

			GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([this, Location, Rotation]()
				{
					AHammer* Hammer = GetWorld()->SpawnActor<AHammer>(AHammer::StaticClass(), Location, Rotation, FActorSpawnParameters());
				}), RandomDelay, false);
		}
	}
}

void AMonsterBoss::DashAttack(AActor* Target)
{
	auto Movement = GetCharacterMovement();
	Movement->SetMovementMode(EMovementMode::MOVE_Flying);
	Movement->MaxFlySpeed = 2000.0f;
	Movement->MaxAcceleration = 4000.0f;
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("OverlapAttack"));
	AIController->ClearFocus(EAIFocusPriority::Gameplay);

	if (DashAttackBodyComponent) {
		DashAttackBodyComponent->DestroyComponent();
	}
	if (DashAttackTrailComponent) {
		DashAttackTrailComponent->DestroyComponent();
	}

	DashAttackBodyComponent = UGameplayStatics::SpawnEmitterAttached(DashAttackBodyEffect, GetMesh(), TEXT("Spine1"));
	DashAttackTrailComponent = UGameplayStatics::SpawnEmitterAttached(DashAttackTrailEffect, GetMesh(), TEXT("Spine1"));
	DashAttackBodyComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
	DashAttackTrailComponent->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));

	FVector TargetUnitVector = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), Target->GetActorLocation());
	AIController->TeleportRequestID = AIController->MoveToLocation(Target->GetActorLocation() + TargetUnitVector * 300.0f);
	AnimInstance->StopAllMontages(10);	
	AnimInstance->PlayAttackMontage();
}

void AMonsterBoss::SetArrowComponent()
{
	//TArray<FVector> ArrowLocations {
	//	FVector(-85.0f, 0.0f, 120.0f),
	//	FVector(-25.0f, 50.0f, 105.0f),
	//	FVector(0.0f, 85.0f, 85.0f),
	//	FVector(0.0f, 50.0f, 50.0f),
	//	FVector(0.0f, -50.0f, 85.0f),
	//	FVector(0.0f, -80.0f, 105.0f),
	//	FVector(0.0f, -80.0f, 40.0f)
	//};

	//for (int32 i = 0; i < 7; i++) {
	//	FName ArrowName = FName(*FString::Printf(TEXT("Arrow%d"), i));
	//	auto Arrows = CreateDefaultSubobject<UArrowComponent>(ArrowName);
	//	Arrows->SetRelativeLocation(ArrowLocations[i]);
	//	Arrows->SetupAttachment(GetCapsuleComponent());
	//	ArrowComponents.Add(Arrows);
	//}
}

void AMonsterBoss::Teleport(FVector _Location)
{
	auto Movement = GetCharacterMovement();
	Movement->SetMovementMode(EMovementMode::MOVE_Flying);
	Movement->MaxFlySpeed = 5000.0f;
	Movement->MaxAcceleration = 99999.0f;
	GetMesh()->SetVisibility(false, true);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));

	if (TeleportBodyComponent) {
		TeleportBodyComponent->DestroyComponent();
	}
	if (TeleportTrailComponent) {
		TeleportTrailComponent->DestroyComponent();
	}
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

	FTimerHandle Waithandle1;
	GetWorld()->GetTimerManager().SetTimer(Waithandle1, FTimerDelegate::CreateLambda([&]() {
		if (TeleportBodyComponent) {
			TeleportBodyComponent->DestroyComponent();
		}
		if (TeleportTrailComponent) {
			TeleportTrailComponent->DestroyComponent();	
		}
	}), 0.8f, false);

	FTimerHandle Waithandle2;
	GetWorld()->GetTimerManager().SetTimer(Waithandle2, FTimerDelegate::CreateLambda([&]() {
		if (DashAttackBodyComponent) {
			DashAttackBodyComponent->DestroyComponent();
		}
		if (DashAttackTrailComponent) {
			DashAttackTrailComponent->DestroyComponent();
		}
	}), 1.2f, false);

}
