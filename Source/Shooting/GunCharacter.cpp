// Fill out your copyright notice in the Description page of Project Settings.


#include "GunCharacter.h"
#include "WeaponGun.h"

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


}

void AGunCharacter::BeginPlay()
{
	Super::BeginPlay();

	auto SpawnWeapon = GetWorld()->SpawnActor<AWeaponGun>(Weapon);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	if (IsValid(SpawnWeapon)) {
		SpawnWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_r_WeaponSocket"));
		SpawnWeapon->SetOwner(this);
	}
}

void AGunCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AGunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
