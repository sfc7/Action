// Fill out your copyright notice in the Description page of Project Settings.


#include "GunCharacter.h"
#include "WeaponGun.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/UnrealMathUtility.h"

AGunCharacter::AGunCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Skeletal(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonWraith/Characters/Heroes/Wraith/Meshes/Wraith.Wraith'"));
	if (Skeletal.Succeeded()) {
		GetMesh()->SetSkeletalMesh(Skeletal.Object);
	}

	static ConstructorHelpers::FClassFinder<AWeaponGun> Gun(TEXT("/Script/Engine.Blueprint'/Game/Shooting/BluePrint/BP_Rifle.BP_Rifle_C'"));
	if (Gun.Succeeded()) {
		Weapon = Gun.Class;
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> Anim(TEXT("/Script/Engine.AnimBlueprint'/Game/Shooting/BluePrint/ABP_Gun.ABP_Gun_C'"));
	if (Anim.Succeeded()) {
		GetMesh()->SetAnimClass(Anim.Class);
	}

	AimTargetLength = 500.0f;
}

void AGunCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	WeaponGun = GetWorld()->SpawnActor<AWeaponGun>(Weapon);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	if (IsValid(WeaponGun)) {
		WeaponGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_r_WeaponSocket"));
		WeaponGun->SetActorRelativeLocation(FVector(-1.156438f, -9.918899f, -1.919678f));
		WeaponGun->SetOwner(this);
	}
	
}

void AGunCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AGunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SpringArm) {
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, AimTargetLength, DeltaTime, 15.0f);
	}
}

void AGunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AGunCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Aiming"), EInputEvent::IE_Pressed, this, &AGunCharacter::AimingStart);
	PlayerInputComponent->BindAction(TEXT("Aiming"), EInputEvent::IE_Released, this, &AGunCharacter::AimingEnd);
}

void AGunCharacter::Attack()
{
	if (ShouldAttack) {
		WeaponGun->Shoot();
	}
}

void AGunCharacter::AimingStart()
{
	IsAiming = true;
	AimTargetLength = 150.f;
}

void AGunCharacter::AimingEnd()
{
	IsAiming = false;
	AimTargetLength = 500.f;
}

float AGunCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float returnDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	

	return returnDamage;
}
