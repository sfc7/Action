// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMonster.h"
#include "CreatureAnimInstance.h"
#include "Components/WidgetComponent.h"
#include "MonsterWidget.h"
#include "MonsterComponent.h"
#include "Components/CapsuleComponent.h" // component 전방선언으로 인해 추가해둠
#include "GameFramework/DefaultPawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GamePlayStatics.h"
#include "Sound/SoundWave.h"

ABaseMonster::ABaseMonster()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, -0.f));

	MonsterComponent = CreateDefaultSubobject<UMonsterComponent>(TEXT("MonsterComponent"));
	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UMonsterWidget> MW(TEXT("/Script/UserWidget'/Game/Shooting/BluePrint/UI/WBP_MonsterHpBar.WBP_MonsterHpBar_C'"));


	if (MW.Succeeded()) {
		HpBar->SetWidgetClass(MW.Class);
		HpBar->SetDrawSize(FVector2D(300.f, 30.f));
		HpBar->SetRelativeLocation(FVector(0.f, 0.f, 180.f));
	}

	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));


}

void ABaseMonster::BeginPlay()
{
	Super::BeginPlay();

	auto HpWidget = Cast<UMonsterWidget>(HpBar->GetUserWidgetObject());
	if (HpWidget) {
		HpWidget->BindHp(MonsterComponent);
	}
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
	if (MonsterComponent->GetHp() > 0) {
		MonsterComponent->OnDamaged(Damage);
		if (IsValid(AnimInstance) && MonsterComponent->GetHp() - (int32)Damage > 0) {
			HitSoundPlay();
			AnimInstance->PlayHitReactMontage();
		}
	}
	return Damage;
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
	}
	else {
		ShouldMove = false;
		ShouldAttack = false;
		IsDamaging = true;
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

