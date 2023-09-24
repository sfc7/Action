// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicCharacter.h"
#include "MagicAnimInstance.h"	
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GamePlayStatics.h"
#include "CharacterUIWidget.h"
#include "PlayerComponent.h"
#include "ShootingGameMode.h"
#include "MagicBall.h"
#include "MagicMeteor.h"
#include "MagicStorm.h"


AMagicCharacter::AMagicCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Skeletal(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonShinbi/Characters/Heroes/Shinbi/Meshes/Shinbi.Shinbi'"));

	if (Skeletal.Succeeded()) {
		GetMesh()->SetSkeletalMesh(Skeletal.Object);
	}

	static ConstructorHelpers::FClassFinder<UCharacterUIWidget> CharacterWidget(TEXT("/Script/UserWidget'/Game/Shooting/BluePrint/UI/WBP_CharacterBar.WBP_CharacterBar_C'"));

	if (CharacterWidget.Succeeded()) {
		CUI = CreateWidget<UCharacterUIWidget>(GetWorld(), CharacterWidget.Class);
		if (IsValid(CUI)) {
			CUI->AddToViewport();
			CUI->SetMagicCharacterInfo();
		}
	}
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> Anim(TEXT("/Script/Engine.AnimBlueprint'/Game/Shooting/BluePrint/ABP_Shinbi.ABP_Shinbi_C'"));
	if (Anim.Succeeded()) {
		GetMesh()->SetAnimClass(Anim.Class);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Portal(TEXT("/Script/Engine.ParticleSystem'/Game/Shooting/BluePrint/Effect/P_Gideon_Meteor_Portal.P_Gideon_Meteor_Portal'"));
	if (Portal.Succeeded()) {
		MeteorPortal = Portal.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Aura(TEXT("/Script/Engine.ParticleSystem'/Game/Shooting/BluePrint/Effect/P_Gideon_Ultimate_Cast.P_Gideon_Ultimate_Cast'"));
	if (Aura.Succeeded()) {
		MeteorAura = Aura.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> AttackBasicSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Character/Magic/SV_Magic_AttackBasic.SV_Magic_AttackBasic'"));
	if (AttackBasicSoundAsset.Succeeded()) {
		AttackBasicSound = AttackBasicSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> AttackBasicSound2Asset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Character/Magic/SV_Magic_AttackBasic2.SV_Magic_AttackBasic2'"));
	if (AttackBasicSound2Asset.Succeeded()) {
		AttackBasicSound2 = AttackBasicSound2Asset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> AttackBasicSound3Asset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Character/Magic/SV_Magic_AttackBasic3.SV_Magic_AttackBasic3'"));
	if (AttackBasicSound3Asset.Succeeded()) {
		AttackBasicSound3 = AttackBasicSound3Asset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> AttackBasicSound4Asset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Character/Magic/SV_Magic_AttackBasic4.SV_Magic_AttackBasic4'"));
	if (AttackBasicSound4Asset.Succeeded()) {
		AttackBasicSound4 = AttackBasicSound4Asset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> HitSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Character/Magic/SV_Magic_Hit.SV_Magic_Hit'"));
	if (HitSoundAsset.Succeeded()) {
		HitSound = HitSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> AttackQSkillSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Character/Magic/SV_Magic_AttacSkillQ.SV_Magic_AttacSkillQ'"));
	if (AttackQSkillSoundAsset.Succeeded()) {
		AttackQSkillSound = AttackQSkillSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> AttackRSkillSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Character/Magic/SV_Magic_AttackSkillR.SV_Magic_AttackSkillR'"));
	if (AttackRSkillSoundAsset.Succeeded()) {
		AttackRSkillSound = AttackRSkillSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> DeathSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Character/Magic/SV_Magic_Death.SV_Magic_Death'"));
	if (DeathSoundAsset.Succeeded()) {
		DeathSound = DeathSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> WeaponSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Character/Magic/SV_Magic_Weapon.SV_Magic_Weapon'"));
	if (WeaponSoundAsset.Succeeded()) {
		WeaponSound = WeaponSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> SpellSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Character/Magic/SV_Magic_Spell.SV_Magic_Spell'"));
	if (SpellSoundAsset.Succeeded()) {
		SpellSound = SpellSoundAsset.Object;
	}

	MaxCombo = 5;
	AttackEndComboState();
}

void AMagicCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (CUI)
	{
		CUI->BindHp(PlayerComponent);
	}
}

void AMagicCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UMagicAnimInstance>(GetMesh()->GetAnimInstance());
	if (IsValid(AnimInstance))
	{
		AnimInstance->OnHit.AddUObject(this, &AMagicCharacter::ToggleDuringHit);
		AnimInstance->DuringAttack.AddUObject(this, &AMagicCharacter::ToggleDuringAttack);
		AnimInstance->DuringDodge.AddUObject(this, &AMagicCharacter::ToggleDuringDodge);
		AnimInstance->AttackBasic.AddUObject(this, &AMagicCharacter::Attack_Basic, 10.f);
		AnimInstance->AttackQSkill.AddUObject(this, &AMagicCharacter::Attack_Q);
		AnimInstance->AttackRSkill.AddUObject(this, &AMagicCharacter::Attack_R);
		AnimInstance->AttackBasicSound.AddUObject(this, &AMagicCharacter::AttackBasicSoundPlay);
		AnimInstance->AttackBasicSound2.AddUObject(this, &AMagicCharacter::AttackBasicSound2Play);
		AnimInstance->AttackBasicSound3.AddUObject(this, &AMagicCharacter::AttackBasicSound3Play);
		AnimInstance->AttackBasicSound4.AddUObject(this, &AMagicCharacter::AttackBasicSound4Play);
	}

	AnimInstance->AttackComboCheck.AddLambda([this]() -> void {
		CanNextCombo = false;

		if (IsComboInputOn)
		{
			AttackStartComboState();
			if (CurrentCombo != MaxCombo) {
				AnimInstance->JumpToAttackMontageSection(CurrentCombo);
			}

		}
	});
	                   
}

void AMagicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMagicCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMagicCharacter::JumpStart);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMagicCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Q_Skill"), EInputEvent::IE_Pressed, this, &AMagicCharacter::Attack_Skill_Q);
	PlayerInputComponent->BindAction(TEXT("R_Skill"), EInputEvent::IE_Pressed, this, &AMagicCharacter::Attack_Skill_R);

	PlayerInputComponent->BindAction(TEXT("ForwardRoll"), EInputEvent::IE_Pressed, this, &AMagicCharacter::Dodge);

	PlayerInputComponent->BindAction(TEXT("CharacterChangeKatana"), EInputEvent::IE_Released, this, &AMagicCharacter::CharacterChangeKatana);


}

void AMagicCharacter::Attack()
{

	if (ShouldAttack)
	{
		if (attacking && FMath::IsWithinInclusive<int>(CurrentCombo, 1, MaxCombo) && CanNextCombo)
		{
			IsComboInputOn = true;
		}
		else {
			if (CurrentCombo == 0) {
				AttackStartComboState();
				AnimInstance->PlayAttackMontage();
				AnimInstance->JumpToAttackMontageSection(CurrentCombo);
				attacking = true;
			}
		}
	}


}

void AMagicCharacter::Attack_Skill_Q()
{
	if (ShouldAttack && Should_Q_Skill)
	{
		AttackQSkillSoundPlay();
		SpellSoundPlay();
		AnimInstance->PlayAttack_Q_SkillMontage();
		Should_Q_Skill = false;

		FTimerHandle waitHandle;
		GetWorld()->GetTimerManager().SetTimer(waitHandle, FTimerDelegate::CreateLambda([&]()
			{
				Should_Q_Skill = true;

			}), 5.0f, false);
	}

}

void AMagicCharacter::Attack_Skill_R()
{

	if (ShouldAttack && Should_R_Skill)
	{
		AttackRSkillSoundPlay();
		SpellSoundPlay();
		UParticleSystemComponent* ParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MeteorAura, GetActorLocation() + FVector(0.0f, 0.f, -100.0f), GetActorRotation());
		if (ParticleComponent) {
			ParticleComponent->SetWorldScale3D(FVector(0.2f, 0.2f, 0.2f));
		}
		AnimInstance->PlayAttack_R_SkillMontage();
		Should_R_Skill = false;

		FTimerHandle waitHandle;
		GetWorld()->GetTimerManager().SetTimer(waitHandle, FTimerDelegate::CreateLambda([&]()
			{
				Should_R_Skill = true;
			}), 20.0f, false);
	}

}

void AMagicCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	CurrentCombo = FMath::Clamp<int>(CurrentCombo + 1, 1, MaxCombo);
}

void AMagicCharacter::AttackEndComboState()
{
	if (CurrentCombo > 0) {
		attacking = false;
		CanNextCombo = false;
		IsComboInputOn = false;
		CurrentCombo = 0;
	}
}

void AMagicCharacter::Attack_Basic(float damage)
{
	FVector StartLocation;
	FVector EndLocation;
	FHitResult HitResult;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	bool bHit;
	float CurrentAngle = -30.f;
	AActor* Target = nullptr;

	WeaponSoundPlay();
	for (int i = 0; i < 60; i++)
	{
		StartLocation = GetActorLocation();

		EndLocation = StartLocation + UKismetMathLibrary::RotateAngleAxis(GetActorForwardVector(), CurrentAngle, FVector(0.0f, 0.0f, 1.0f)) * 1500.0f;
		bHit = GetWorld()->LineTraceSingleByChannel(
			OUT HitResult,
			StartLocation,
			EndLocation,
			ECC_GameTraceChannel5,
			CollisionParams
		);

		if (bHit && HitResult.GetActor()->GetClass()->GetSuperClass()->GetName() == TEXT("BaseMonster")) {
			Target = HitResult.GetActor();
		}
		CurrentAngle++;
	}


	AMagicBall* MagicBall = GetWorld()->SpawnActor<AMagicBall>(AMagicBall::StaticClass(), GetActorLocation(), GetActorRotation(), FActorSpawnParameters());
	if (MagicBall) {
		if (IsValid(Target)) {
			MagicBall->SetTarget(Target);
		}
	}
}

void AMagicCharacter::Attack_R()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MeteorPortal, GetActorLocation()+ GetActorRotation().Vector() * 300.0f + FVector(0.0f, 0.0f, 450.0f), GetActorRotation());

	for (int i = 0; i < 6; i++)
	{
		FTimerHandle waitHandle1;
		FTimerHandle waitHandle2;
		GetWorld()->GetTimerManager().SetTimer(waitHandle1, FTimerDelegate::CreateLambda([&]()
		{
			FVector RandomOffset(FMath::RandRange(-150.f, 0.0f), FMath::RandRange(-250.f, 250.f), 0.f);
			GetWorld()->SpawnActor<AMagicMeteor>(AMagicMeteor::StaticClass(), GetActorLocation()+ GetActorRotation().Vector() * 300.0f + FVector(0.0f, 0.0f, 450.0f) + RandomOffset, GetActorRotation(), FActorSpawnParameters());

		}), 0.35f * i, false);
		GetWorld()->GetTimerManager().SetTimer(waitHandle2, FTimerDelegate::CreateLambda([&]()
		{
			FVector RandomOffset(FMath::RandRange(0.f, 150.0f), FMath::RandRange(-250.f, 250.f), 0.f);
			GetWorld()->SpawnActor<AMagicMeteor>(AMagicMeteor::StaticClass(), GetActorLocation() + GetActorRotation().Vector() * 300.0f + FVector(0.0f, 0.0f, 450.0f) + RandomOffset, GetActorRotation(), FActorSpawnParameters());

		}), 0.45f * i, false);
	}
}

void AMagicCharacter::Attack_Q()
{
	GetWorld()->SpawnActor<AMagicStorm>(AMagicStorm::StaticClass(), GetActorLocation() + FVector(50.f, 0.f, -50.f), GetActorRotation(), FActorSpawnParameters());
}

float AMagicCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float returnDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (PlayerComponent->GetHp() > 0) {
		PlayerComponent->OnDamaged(returnDamage);
		if (IsValid(AnimInstance) && PlayerComponent->GetHp() - (int32)returnDamage > 0) {
			HitSoundPlay();
			AnimInstance->PlayHitReactMontage();
		}
	}
	return returnDamage;
}

void AMagicCharacter::Dodge()
{
	if (ShouldDodge) {
		AnimInstance->PlayForwardRollMontage();
		GetCharacterMovement()->Velocity = GetActorForwardVector() * MoveDistance;
	}
}

void AMagicCharacter::Death()
{
	ToggleAllAct();	
	DeathSoundPlay();
	CUI->RemoveFromParent();

	TArray<AActor*> actors;
	GetAttachedActors(actors);
	for (auto actor : actors) {
		actor->Destroy();
	}

	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
	GetMesh()->SetCollisionProfileName("NoCollision");

	if (IsValid(AnimInstance)) {
		AnimInstance->StopAllMontages(0.0f);
		AnimInstance->PlayDeathMontage();
	}

	FTimerHandle waitHandle;
	GetWorld()->GetTimerManager().SetTimer(waitHandle, FTimerDelegate::CreateLambda([&]()
		{
			Destroy();
		}), 5.0f, false);

	AShootingGameMode* gameMode = GetWorld()->GetAuthGameMode<AShootingGameMode>();
	if (gameMode != nullptr) {
		gameMode->GameOver(this);
	}
}

void AMagicCharacter::AttackBasicSound2Play()
{
	if (IsValid(AttackBasicSound2)) {
		UGameplayStatics::PlaySoundAtLocation(this, AttackBasicSound2, GetActorLocation());
	}
}

void AMagicCharacter::AttackBasicSound3Play()
{
	if (IsValid(AttackBasicSound3)) {
		UGameplayStatics::PlaySoundAtLocation(this, AttackBasicSound3, GetActorLocation());
	}
}

void AMagicCharacter::AttackBasicSound4Play()
{
	if (IsValid(AttackBasicSound4)) {
		UGameplayStatics::PlaySoundAtLocation(this, AttackBasicSound4, GetActorLocation());
	}
}

void AMagicCharacter::SpellSoundPlay()
{
	if (IsValid(SpellSound)) {
		UGameplayStatics::PlaySoundAtLocation(this, SpellSound, GetActorLocation());
	}
}
