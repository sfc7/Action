#include "KatanaCharacter.h"
#include "KatanaAnimInstance.h"	
#include "Sound/SoundWave.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GamePlayStatics.h"
#include "CharacterUIWidget.h"
#include "PlayerComponent.h"

AKatanaCharacter::AKatanaCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Skeletal(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonGideon/Characters/Heroes/Gideon/Skins/Mephisto/Meshes/Gideon_Mephisto.Gideon_Mephisto'"));

	if (Skeletal.Succeeded()) {
		GetMesh()->SetSkeletalMesh(Skeletal.Object);
	}

	static ConstructorHelpers::FClassFinder<UCharacterUIWidget> CharacterWidget(TEXT("/Script/UserWidget'/Game/Shooting/BluePrint/UI/WBP_CharacterBar.WBP_CharacterBar_C'"));

	if (CharacterWidget.Succeeded()) {
		CUI = CreateWidget<UCharacterUIWidget>(GetWorld(), CharacterWidget.Class);
		if (IsValid(CUI)) {
			CUI->AddToViewport();
			CUI->SetKatanaCharacterInfo();
		}
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> Anim(TEXT("/Script/Engine.AnimBlueprint'/Game/Shooting/BluePrint/ABP_Character.ABP_Character_C'"));
	if (Anim.Succeeded()) {
		GetMesh()->SetAnimClass(Anim.Class);
	}

	static ConstructorHelpers::FClassFinder<AActor> Sword(TEXT("/Script/Engine.Blueprint'/Game/Shooting/BluePrint/BP_Sword.BP_Sword_C'"));
	if (Sword.Succeeded())
	{
		Weapon = Sword.Class;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Attackeffect(TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_hit2.P_ky_hit2'"));
	if (Attackeffect.Succeeded()) {
		HitEffect = Attackeffect.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> AttackBasicSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Character/Katana/SV_Katana_AttackBasic.SV_Katana_AttackBasic'"));
	if (AttackBasicSoundAsset.Succeeded()) {
		AttackBasicSound = AttackBasicSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> AttackBasicSound2Asset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Character/Katana/SV_Katana_AttackBasic2.SV_Katana_AttackBasic2'"));
	if (AttackBasicSound2Asset.Succeeded()) {
		AttackBasicSound2 = AttackBasicSound2Asset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> AttackBasicSound3Asset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Character/Katana/SV_Katana_AttackBasic3.SV_Katana_AttackBasic3'"));
	if (AttackBasicSound3Asset.Succeeded()) {
		AttackBasicSound3 = AttackBasicSound3Asset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> HitSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Character/Katana/SV_Katana_Hit.SV_Katana_Hit'"));
	if (HitSoundAsset.Succeeded()) {
		HitSound = HitSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> AttackQSkillSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Character/Katana/SV_Katana_AttackQSKill.SV_Katana_AttackQSKill'"));
	if (AttackQSkillSoundAsset.Succeeded()) {
		AttackQSkillSound = AttackQSkillSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> AttackRSkillSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Character/Katana/SV_Katana_AttackRSKill.SV_Katana_AttackRSKill'"));
	if (AttackRSkillSoundAsset.Succeeded()) {
		AttackRSkillSound = AttackRSkillSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> DeathSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Character/Katana/SV_Katana_Death.SV_Katana_Death'"));
	if (DeathSoundAsset.Succeeded()) {
		DeathSound = DeathSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> WeaponSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Character/Katana/SV_Katana_Weapon.SV_Katana_Weapon'"));
	if (WeaponSoundAsset.Succeeded()) {
		WeaponSound = WeaponSoundAsset.Object;
	}

	MaxCombo = 4;
	AttackEndComboState();
}

void AKatanaCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(IsValid(Weapon))
	{
		FName socket = "hand_r_WeaponSocket";
		FActorSpawnParameters SpawnParams;

		auto SpawnWeapon = GetWorld()->SpawnActor<AActor>(Weapon, FVector(-8.f, 3.f, 0.f), FRotator(180.f, 10.f, -170.f), SpawnParams);
		if (IsValid(SpawnWeapon)) {
			SpawnWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, socket);
		}
	}

	if (CUI)
	{
		CUI->BindHp(PlayerComponent);
	}
}

void AKatanaCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UKatanaAnimInstance>(GetMesh()->GetAnimInstance());
	if (IsValid(AnimInstance))
	{
		AnimInstance->OnHit.AddUObject(this, &AKatanaCharacter::ToggleDuringHit);
		AnimInstance->DuringAttack.AddUObject(this, &AKatanaCharacter::ToggleDuringAttack);
		AnimInstance->DuringDodge.AddUObject(this, &AKatanaCharacter::ToggleDuringDodge);
		AnimInstance->AttackBasic.AddUObject(this, &AKatanaCharacter::Attack_Basic,10.f);
		AnimInstance->AttackQSkill.AddUObject(this, &AKatanaCharacter::Attack_Basic, 20.f);
		AnimInstance->AttackRSkill.AddUObject(this, &AKatanaCharacter::Attack_Basic, 10.f);
		AnimInstance->AttackBasicSound.AddUObject(this, &AKatanaCharacter::AttackBasicSoundPlay);
		AnimInstance->AttackBasicSound2.AddUObject(this, &AKatanaCharacter::AttackBasicSound2Play);
		AnimInstance->AttackBasicSound3.AddUObject(this, &AKatanaCharacter::AttackBasicSound3Play);
		AnimInstance->AttackRSkillSound.AddUObject(this, &AKatanaCharacter::AttackRSkillSoundPlay);
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

void AKatanaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKatanaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AKatanaCharacter::JumpStart);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AKatanaCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Q_Skill"), EInputEvent::IE_Pressed, this, &AKatanaCharacter::Attack_Skill_Q);
	PlayerInputComponent->BindAction(TEXT("R_Skill"), EInputEvent::IE_Pressed, this, &AKatanaCharacter::Attack_Skill_R);

	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AKatanaCharacter::SprintStart);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AKatanaCharacter::SprintEnd);

	PlayerInputComponent->BindAction(TEXT("ForwardRoll"), EInputEvent::IE_Pressed, this, &AKatanaCharacter::Dodge);

	PlayerInputComponent->BindAction(TEXT("CharacterChangeMagic"), EInputEvent::IE_Released, this, &AKatanaCharacter::CharacterChangeMagic);
}

void AKatanaCharacter::Attack()
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

void AKatanaCharacter::Attack_Skill_Q()
{
	if (ShouldAttack && Should_Q_Skill)
	{
		AttackQSkillSoundPlay();
		AnimInstance->PlayAttack_Q_SkillMontage();
		Should_Q_Skill = false;

		FTimerHandle waitHandle;
		GetWorld()->GetTimerManager().SetTimer(waitHandle, FTimerDelegate::CreateLambda([&]()
		{
				Should_Q_Skill = true;
				
		}), 5.0f, false);
	}

}

void AKatanaCharacter::Attack_Skill_R()
{
	if (ShouldAttack && Should_R_Skill)
	{
		AnimInstance->PlayAttack_R_SkillMontage();
		Should_R_Skill = false;

		FTimerHandle waitHandle;
		GetWorld()->GetTimerManager().SetTimer(waitHandle, FTimerDelegate::CreateLambda([&]()
			{
				Should_R_Skill = true;
			}), 20.0f, false);
	}
}

void AKatanaCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	CurrentCombo = FMath::Clamp<int>(CurrentCombo + 1, 1, MaxCombo);
}

void AKatanaCharacter::AttackEndComboState()
{
	if (CurrentCombo > 0) {
		attacking = false;
		CanNextCombo = false;
		IsComboInputOn = false;
		CurrentCombo = 0;
	}
}

void AKatanaCharacter::Attack_Basic(float damage)
{
	float AttackRange = 100.f;
	float AttackRadius = 50.f;

	UE_LOG(LogTemp, Log, TEXT("%f"), damage);
	FVector Center = GetActorLocation();

	FVector Forward = Center + GetActorForwardVector() * AttackRange;
	FCollisionQueryParams Params(NAME_None ,false, this);

	TArray<FHitResult> HitResults;

	bool Result = GetWorld()->SweepMultiByChannel(
		OUT HitResults,
		Center,
		Forward,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat();
	FColor DrawColor;

	for (FHitResult HitResult : HitResults) {
		if (Result && HitResult.GetActor()->GetClass()->GetSuperClass()->GetName() == TEXT("BaseMonster"))
		{
			DrawColor = FColor::Green;
			WeaponSoundPlay();
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, HitResult.ImpactPoint);

			AActor* HitActor = HitResult.GetActor();
			UGameplayStatics::ApplyDamage(HitActor, damage, GetController(), HitActor, NULL);
		}
		else {
			DrawColor = FColor::Red;
		}
	}


	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, Rotation, DrawColor, false, 3.f);
}

float AKatanaCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (PlayerComponent->GetHp() > 0) {
		PlayerComponent->OnDamaged(Damage);
		if (IsValid(AnimInstance) && PlayerComponent->GetHp() - (int32)Damage > 0) {
			HitSoundPlay();
			AnimInstance->PlayHitReactMontage();
		}
	}
	return Damage;
}

void AKatanaCharacter::Dodge()
{
	if (ShouldDodge) {
		AnimInstance->PlayForwardRollMontage();
		GetCharacterMovement()->Velocity = GetActorForwardVector() * MoveDistance;
	}
}

void AKatanaCharacter::Death()
{
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
}

void AKatanaCharacter::AttackBasicSound2Play()
{
	if (IsValid(AttackBasicSound2)) {
		UGameplayStatics::PlaySoundAtLocation(this, AttackBasicSound2, GetActorLocation());
	}
}

void AKatanaCharacter::AttackBasicSound3Play()
{
	if (IsValid(AttackBasicSound3)) {
		UGameplayStatics::PlaySoundAtLocation(this, AttackBasicSound3, GetActorLocation());
	}
}



