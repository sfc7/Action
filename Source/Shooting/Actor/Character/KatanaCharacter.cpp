#include "KatanaCharacter.h"
#include "KatanaAnimInstance.h"	
#include "Sound/SoundWave.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GamePlayStatics.h"
#include "CharacterUIWidget.h"
#include "ShootingGameMode.h"
#include "PlayerComponent.h"
#include "MyGameInstance.h"

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

	static ConstructorHelpers::FObjectFinder<USoundWave> AttackBasicSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Character/Katana/SW_Katana_AttackBasic.SW_Katana_AttackBasic'"));
	if (AttackBasicSoundAsset.Succeeded()) {
		AttackBasicSound = AttackBasicSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> AttackBasicSound2Asset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Character/Katana/SW_Katana_AttackBasic2.SW_Katana_AttackBasic2'"));
	if (AttackBasicSound2Asset.Succeeded()) {
		AttackBasicSound2 = AttackBasicSound2Asset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> AttackBasicSound3Asset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Character/Katana/SW_Katana_AttackBasic3.SW_Katana_AttackBasic3'"));
	if (AttackBasicSound3Asset.Succeeded()) {
		AttackBasicSound3 = AttackBasicSound3Asset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> HitSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Character/Katana/SW_Katana_Hit.SW_Katana_Hit'"));
	if (HitSoundAsset.Succeeded()) {
		HitSound = HitSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> AttackQSkillSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Character/Katana/SW_Katana_AttackQSKill.SW_Katana_AttackQSKill'"));
	if (AttackQSkillSoundAsset.Succeeded()) {
		AttackQSkillSound = AttackQSkillSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> AttackRSkillSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Character/Katana/SW_Katana_AttackRSKill.SW_Katana_AttackRSKill'"));
	if (AttackRSkillSoundAsset.Succeeded()) {
		AttackRSkillSound = AttackRSkillSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> DeathSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Character/Katana/SW_Katana_Death.SW_Katana_Death'"));
	if (DeathSoundAsset.Succeeded()) {
		DeathSound = DeathSoundAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> WeaponSoundAsset(TEXT("/Script/Engine.SoundWave'/Game/Shooting/BluePrint/Sound/Character/Katana/SW_Katana_Weapon.SW_Katana_Weapon'"));
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

	if (MyGameInstance) {
		Should_Q_Cooldown = MyGameInstance->Katana_Q_Cooldown;
		Should_Q_Skill = MyGameInstance->Katana_Should_Q_Skill;
		Should_R_Cooldown = MyGameInstance->Katana_R_Cooldown;
		Should_R_Skill = MyGameInstance->Katana_Should_R_Skill;
	}
}

void AKatanaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AKatanaCharacter::JumpStart);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AKatanaCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Q_Skill"), EInputEvent::IE_Pressed, this, &AKatanaCharacter::Attack_Skill_Q);
	PlayerInputComponent->BindAction(TEXT("R_Skill"), EInputEvent::IE_Pressed, this, &AKatanaCharacter::Attack_Skill_R);

	PlayerInputComponent->BindAction(TEXT("ForwardRoll"), EInputEvent::IE_Pressed, this, &AKatanaCharacter::Dodge);

	PlayerInputComponent->BindAction(TEXT("CharacterChangeMagic"), EInputEvent::IE_Released, this, &AKatanaCharacter::CharacterChangeMagic);
	PlayerInputComponent->BindAction(TEXT("CharacterChangeGun"), EInputEvent::IE_Released, this, &AKatanaCharacter::CharacterChangeGun);
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
		MyGameInstance->Katana_Q_SkillCoolDownStart();
	}

}

void AKatanaCharacter::Attack_Skill_R()
{
	if (ShouldAttack && Should_R_Skill)
	{
		AnimInstance->PlayAttack_R_SkillMontage();
		Should_R_Skill = false; 
		MyGameInstance->Katana_R_SkillCoolDownStart();
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
	float returnDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (PlayerComponent->GetHp() > 0) {
		PlayerComponent->OnDamaged(returnDamage);
		if (IsValid(AnimInstance) && PlayerComponent->GetHp() - (int32)returnDamage > 0) {
			HitSoundPlay();
			AnimInstance->PlayHitReactMontage();
			
		}
	}
	return  returnDamage;
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
	ToggleAllAct();
	DeathSoundPlay();
	CUI->RemoveFromParent();

	TArray<AActor*> actors;
	GetAttachedActors(actors);
	for (auto actor : actors) {
		actor->Destroy();
	}


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



