// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMonster.h"
#include "CreatureAnimInstance.h"
#include "MonsterAIController.h"
#include "MonsterComponent.h"
#include "Components/CapsuleComponent.h" 
#include "GameFramework/DefaultPawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GamePlayStatics.h"
#include "Sound/SoundWave.h"
#include "GameFramework/CharacterMovementComponent.h"

ABaseMonster::ABaseMonster()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, -0.f));

	MonsterComponent = CreateDefaultSubobject<UMonsterComponent>(TEXT("MonsterComponent"));
	

	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));

	GetMesh()->bRenderCustomDepth = true;
}

void ABaseMonster::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AMonsterAIController>(GetController());
}

void ABaseMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UCreatureAnimInstance>(GetMesh()->GetAnimInstance());
	MonsterComponent->OnDeath.AddUObject(this, &ABaseMonster::Death);
}

float ABaseMonster::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float returnDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (MonsterComponent->GetHp() > 0) {
		MonsterComponent->OnDamaged(returnDamage);
		if (IsValid(AnimInstance) && MonsterComponent->GetHp() - (int32)returnDamage > 0) {
			HitSoundPlay();
			AnimInstance->PlayHitReactMontage();
		}
	}
	return returnDamage;
}

void ABaseMonster::ToggleDuringAttack(bool enable)
{
	if (enable) {
		ShouldMove = true;
		IsAttacking = false;
	}
	else {
		ShouldMove = false;
		IsAttacking = true;
	}
}

void ABaseMonster::ToggleDuringHit(bool enable)
{
	if (enable) {
		ShouldMove = true;
		ShouldAttack = true;
		IsDamaging = false;
		IsAttacking = false;
		GetCharacterMovement()->StopMovementImmediately();
		if (AIController) {
			AIController->SetIsDamaging(false);
		}
	}
	else {
		ShouldMove = false;
		ShouldAttack = false;
		IsDamaging = true;
		if (AIController) {
			AIController->SetIsDamaging(true);
		}
	}
}

void ABaseMonster::Death()
{
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
	GetMesh()->SetCollisionProfileName("NoCollision");

	if (IsValid(AnimInstance)) {
		DeathSoundPlay();
		AnimInstance->StopAllMontages(0.0f);
		AnimInstance->PlayDeathMontage();
	}


	FTimerHandle waitHandle;
	GetWorld()->GetTimerManager().SetTimer(waitHandle, FTimerDelegate::CreateLambda([&]()
		{
			Destroy();
		}), 5.0f, false);
}

void ABaseMonster::SetAttackRotation(AActor* Target)
{
	FRotator FindRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target->GetActorLocation());
	FRotator TargetRotation = FRotator(0.0f, FindRotation.Yaw, 0.0f);
	
	if (Target) {
		SetActorRotation(TargetRotation);
	}
}

void ABaseMonster::HitSoundPlay()
{
	if (IsValid(HitSound)) {
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}
}

void ABaseMonster::AttackBasicSoundPlay()
{
	if (IsValid(AttackBasicSound)) {

		UGameplayStatics::PlaySoundAtLocation(this, AttackBasicSound, GetActorLocation());
	}
}

void ABaseMonster::DeathSoundPlay()
{
	if (IsValid(DeathSound)) {
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}
}

