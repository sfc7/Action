	// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "CharacterUIWidget.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GamePlayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MagicCharacter.h"
#include "KatanaCharacter.h"
#include "PlayerComponent.h"
#include "BasePlayerController.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	PlayerComponent = CreateDefaultSubobject<UPlayerComponent>(TEXT("PlayerComponent"));

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, -0.f));
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Character"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = 500.f;
	SpringArm->SetRelativeRotation(FRotator(-35.f, 0.f, 0.f));
	SpringArm->SocketOffset = (FVector(0.f, 80.f, 75.f));
	SpringArm->bUsePawnControlRotation = true;


	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->bOrientRotationToMovement = true;


	static ConstructorHelpers::FObjectFinder<UParticleSystem> SpawnEffectAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Shooting/BluePrint/Effect/P_Portal_Entrance_Close.P_Portal_Entrance_Close'"));
	if (SpawnEffectAsset.Succeeded()) {
		SpawnEffect = SpawnEffectAsset.Object;
	}

	

}

// Called when the game starts or w	hen spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	BaseController = Cast<APlayerController>(GetController());
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveUpDown"), this, &ABaseCharacter::MoveUpDown);
	PlayerInputComponent->BindAxis(TEXT("MoveLeftRight"), this, &ABaseCharacter::MoveLeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUpDown"), this, &ABaseCharacter::LookUpDown);
	PlayerInputComponent->BindAxis(TEXT("LookLeftRight"), this, &ABaseCharacter::LookLeftRight);

	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &ABaseCharacter::SprintStart);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &ABaseCharacter::SprintEnd);
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PlayerComponent->OnDeath.AddUObject(this, &ABaseCharacter::Death);
}

void ABaseCharacter::MoveUpDown(float value)
{
	if (ShouldMove) {
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);
	}
}

void ABaseCharacter::MoveLeftRight(float value)
{
	if (ShouldMove) {
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, value);
	}
}

void ABaseCharacter::LookUpDown(float value)
{
	AddControllerPitchInput(value);
}

void ABaseCharacter::LookLeftRight(float value)
{
	AddControllerYawInput(value);
}

void ABaseCharacter::ToggleDuringAttack(bool enable)
{
	if (enable) {
		ShouldMove = true;
		ShouldJump = true;
		IsAttacking = false;
		AttackEndComboState();
	}
	else {
		ShouldMove = false;
		ShouldJump = false;
		IsAttacking = true;
	}
}

void ABaseCharacter::ToggleDuringHit(bool enable)
{
	if (enable) {
		ShouldMove = true;
		ShouldAttack = true;
		ShouldDodge = true;
		IsDamaging = false;
		IsAttacking = false;
		AttackEndComboState();
	}
	else {
		ShouldMove = false;
		ShouldAttack = false;
		ShouldDodge = false;
		IsDamaging = true;
	}
}

void ABaseCharacter::ToggleDuringDodge(bool enable)
{
	if (enable) {
		ShouldMove = true;
		ShouldAttack = true;
		ShouldDodge = true;
		IsDamaging = false;
		IsAttacking = false;
		AttackEndComboState();
	}
	else {
		ShouldMove = false;
		ShouldDodge = false;
		IsDamaging = true;
	}
}

void ABaseCharacter::ToggleAllAct()
{
	ShouldMove = false;
	ShouldAttack = false;
	ShouldJump = false;
	ShouldDodge = false;
	Should_Q_Skill = false;
	Should_R_Skill = false;
}

void ABaseCharacter::AttackStartComboState()
{
}

void ABaseCharacter::AttackEndComboState()
{
}

void ABaseCharacter::JumpStart()
{
	if (ShouldJump) {
		Jump();
	}

}

void ABaseCharacter::CharacterChangeMagic()
{
	auto CharacterLocation = GetActorLocation();
	auto CharacterRotation = GetActorRotation();

	CUI->RemoveFromParent();

	TArray<AActor*> actors;
	GetAttachedActors(actors);
	for (auto actor : actors) {
		actor->Destroy();
	}

	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	auto magiccharacter = GetWorld()->SpawnActor<AMagicCharacter>(
		AMagicCharacter::StaticClass(),
		CharacterLocation,
		CharacterRotation,
		SpawnParams
	);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnEffect, CharacterLocation, CharacterRotation);
	
	if (magiccharacter) {
		GetController()->Possess(magiccharacter);
	}
	
	
	Destroy();
}

void ABaseCharacter::CharacterChangeKatana()
{
	auto CharacterLocation = GetActorLocation();
	auto CharacterRotation = GetActorRotation();

	CUI->RemoveFromParent();

	TArray<AActor*> actors;
	GetAttachedActors(actors);
	for (auto actor : actors) {
		actor->Destroy();
	}

	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	auto katanacharacter = GetWorld()->SpawnActor<AKatanaCharacter>(
		AKatanaCharacter::StaticClass(),
		CharacterLocation,
		CharacterRotation,
		SpawnParams
	);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnEffect, CharacterLocation, CharacterRotation);

	if (katanacharacter) {
		GetController()->Possess(katanacharacter);
	}

	Destroy();
}

void ABaseCharacter::Death()
{

}

void ABaseCharacter::AttackBasicSoundPlay()
{
	if (IsValid(AttackBasicSound)) {

		UGameplayStatics::PlaySoundAtLocation(this, AttackBasicSound, GetActorLocation());
	}
}

void ABaseCharacter::AttackQSkillSoundPlay()
{
	if (IsValid(AttackQSkillSound)) {
		UGameplayStatics::PlaySoundAtLocation(this, AttackQSkillSound, GetActorLocation());
	}
}

void ABaseCharacter::AttackRSkillSoundPlay()
{
	if (IsValid(AttackRSkillSound)) {
		UGameplayStatics::PlaySoundAtLocation(this, AttackRSkillSound, GetActorLocation());
	}
}

void ABaseCharacter::HitSoundPlay()
{
	if (IsValid(HitSound)) {
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}
}

void ABaseCharacter::DeathSoundPlay()
{
	if (IsValid(DeathSound)) {
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}
}

void ABaseCharacter::WeaponSoundPlay()
{
	if (IsValid(WeaponSound)) {
		UGameplayStatics::PlaySoundAtLocation(this, WeaponSound, GetActorLocation());
	}
}

void ABaseCharacter::SprintStart()
{
	if (ShouldRun) {
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		IsRunning = true;
	}

}

void ABaseCharacter::SprintEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	IsRunning = false;
}


