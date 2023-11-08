// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterDragon.h"
#include "Monster_Dragon_AIController.h"
#include "MonsterDragonAnimInstance.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FireBall.h"
#include "FireBreath.h"
#include "GameFramework/CharacterMovementComponent.h"

AMonsterDragon::AMonsterDragon()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Skeletal(TEXT("/Script/Engine.SkeletalMesh'/Game/Shooting/BluePrint/Mesh/Monster/Dragon/SK_MOUNTAIN_DRAGON.SK_MOUNTAIN_DRAGON'"));
	if (Skeletal.Succeeded()) {
		GetMesh()->SetSkeletalMesh(Skeletal.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> Anim(TEXT("/Script/Engine.AnimBlueprint'/Game/Shooting/BluePrint/ABP_Dragon.ABP_Dragon_C'"));
	if (Anim.Succeeded()) {
		GetMesh()->SetAnimClass(Anim.Class);
	}

	GetMesh()->SetRelativeScale3D(FVector(1.3f, 1.3f, 1.3f));
	AIControllerClass = AMonster_Dragon_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	UCapsuleComponent* MyCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule5"));
	MyCapsuleComponent->SetRelativeScale3D(FVector(2.0f, 3.0f, 3.0f));
	MyCapsuleComponent->SetRelativeLocationAndRotation(FVector(0.0f, 30.0f, 0.0f), FRotator(90.0f, 0.0f, 60.0f));
	MyCapsuleComponent->SetCollisionProfileName(TEXT("Monster"));
	if (IsValid(MyCapsuleComponent))
	{
		FName Socket = TEXT("MOUNTAIN_DRAGON_-Head");
		MyCapsuleComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, Socket);
	}

	MyCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule6"));
	MyCapsuleComponent->SetRelativeScale3D(FVector(2.0f, 3.0f, 7.0f));
	MyCapsuleComponent->SetRelativeLocationAndRotation(FVector(400.0f, 0.0f, 0.0f), FRotator(90.0f, 0.0f, -0.0f));
	MyCapsuleComponent->SetCollisionProfileName(TEXT("Monster"));
	if (IsValid(MyCapsuleComponent))
	{
		FName Socket = TEXT("MOUNTAIN_DRAGON_-Tail");
		MyCapsuleComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, Socket);
	}

	MyCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleSpine"));
	MyCapsuleComponent->SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));
	MyCapsuleComponent->SetRelativeLocationAndRotation(FVector(100.0f, -100.0f, 0.0f), FRotator(90.0f, 0.0f, 0.0f));
	MyCapsuleComponent->SetCollisionProfileName(TEXT("Monster"));
	if (IsValid(MyCapsuleComponent))
	{
		FName Socket = TEXT("MOUNTAIN_DRAGON_-Spine");
		MyCapsuleComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, Socket);
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> AttackBasicSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Monster/Dragon/SW_Dragon_Attack.SW_Dragon_Attack'"));
	if (AttackBasicSoundAsset.Succeeded()) {
		AttackBasicSound = AttackBasicSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> HitSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Monster/Dragon/SW_Dragon_Hit.SW_Dragon_Hit'"));
	if (HitSoundAsset.Succeeded()) {
		HitSound = HitSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> DeathSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Monster/Dragon/SW_Dragon_Death.SW_Dragon_Death'"));
	if (DeathSoundAsset.Succeeded()) {
		DeathSound = DeathSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> FireBallSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Monster/Dragon/SW_Dragon_FireBall.SW_Dragon_FireBall'"));
	if (FireBallSoundAsset.Succeeded()) {
		FireBallSound = FireBallSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> FireBreathSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Monster/Dragon/SW_Dragon_FireBreath.SW_Dragon_FireBreath'"));
	if (FireBreathSoundAsset.Succeeded()) {
		FireBreathSound = FireBreathSoundAsset.Object;
	}
}

void AMonsterDragon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UMonsterDragonAnimInstance>(GetMesh()->GetAnimInstance());
	if (IsValid(AnimInstance)) {
		AnimInstance->DuringAttack.AddUObject(this, &AMonsterDragon::ToggleDuringAttack);
		AnimInstance->AttackBite.AddUObject(this, &AMonsterDragon::Attack_Bite, 20.0f);
		AnimInstance->AttackLClaw.AddUObject(this, &AMonsterDragon::Attack_LClaw, 10.0f);
		AnimInstance->AttackRClaw.AddUObject(this, &AMonsterDragon::Attack_RClaw, 10.0f);
		AnimInstance->AttackFireball.AddUObject(this, &AMonsterDragon::Spawn_Fireball);
		AnimInstance->AttackFirebreath.AddUObject(this, &AMonsterDragon::Spawn_Firebreath);
	}
}

void AMonsterDragon::BeginPlay()
{
	Super::BeginPlay();
}

void AMonsterDragon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMonsterDragon::Attack(AActor* Target)
{
	if (!IsAttacking && ShouldAttack) {
		SetAttackRotation(Target);
		AnimInstance->PlayAttackMontage();
	}
}

void AMonsterDragon::Attack_Bite(float damage)
{
	AttackBasicSoundPlay();

	AttackChannel("MOUNTAIN_DRAGON_-Head", damage);
}

void AMonsterDragon::Attack_LClaw(float damage)
{
	AttackBasicSoundPlay();

	AttackChannel("MOUNTAIN_DRAGON_-L-Hand", damage);
}

void AMonsterDragon::Attack_RClaw(float damage)
{
	AttackBasicSoundPlay();

	AttackChannel("MOUNTAIN_DRAGON_-R-Hand", damage);
}

void AMonsterDragon::RangeAttack(AActor* Target)
{
	if (!IsAttacking && ShouldAttack) {
		TargetLocation = Target->GetActorLocation();
		SetAttackRotation(Target);
		AnimInstance->PlayRangeAttackMontage();
	}
}

void AMonsterDragon::Spawn_Fireball()
{
	FireBallSoundPlay();
	FName SocketName = "MOUNTAIN_DRAGON_-Head";
	AFireBall* Fireball = GetWorld()->SpawnActor<AFireBall>(AFireBall::StaticClass(), GetMesh()->GetSocketLocation(SocketName), GetActorRotation(), FActorSpawnParameters());
	if (Fireball) {
		Fireball->SetTargetLocation(TargetLocation);
	}
}

void AMonsterDragon::Spawn_Firebreath()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	FireBreathSoundPlay();
	FName SocketName = "MOUNTAIN_DRAGON_-Head";
	AFireBreath* Firebreath = GetWorld()->SpawnActor<AFireBreath>(AFireBreath::StaticClass(), GetMesh()->GetSocketLocation(SocketName) + FVector(0.0f,300.0f,0.0f), GetActorRotation(), SpawnParams);
	if (Firebreath) {
		Firebreath->SetTargetLocation(TargetLocation);
	}
}

void AMonsterDragon::AttackChannel(FName _SocketName, float _Damage)
{
	FHitResult HitResult;
	float AttackRange = -100.0f;
	float AttackRadius = 250.f;

	FName SocketName = _SocketName;
	FVector Center = GetMesh()->GetSocketLocation(SocketName);

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


		ACharacter* HitActor = Cast<ACharacter>(HitResult.GetActor());
		UGameplayStatics::ApplyDamage(HitActor, _Damage, GetController(), HitActor, NULL);

		FVector LaunchDirection = (HitActor->GetActorLocation() - Forward).GetSafeNormal();
		LaunchDirection.Z = 0.5f; 
		float LaunchStrength = 800.0f; 

		HitActor->GetCharacterMovement()->Launch(LaunchDirection * LaunchStrength);
	}
	else {
		DrawColor = FColor::Red;
	}

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, Rotation, DrawColor, false, 3.f);
}

void AMonsterDragon::FireBreathSoundPlay()
{
	if (IsValid(FireBreathSound)) {
		UGameplayStatics::PlaySoundAtLocation(this, FireBreathSound, GetActorLocation());
	}
}

void AMonsterDragon::FireBallSoundPlay()
{
	if (IsValid(FireBallSound)) {
		UGameplayStatics::PlaySoundAtLocation(this, FireBallSound, GetActorLocation());
	}
}


	