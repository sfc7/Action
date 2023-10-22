// Fill out your copyright notice in the Description page of Project Settings.


#include "GunCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Math/UnrealMathUtility.h"
#include "GunAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "WeaponGun.h"
#include "Missle.h"

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

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SkillCircle(TEXT("/Script/Engine.StaticMesh'/Game/Shooting/BluePrint/Effect/Gun/SM_Sphere.SM_Sphere'"));
	if (SkillCircle.Succeeded()) {
		RSkillCircle = SkillCircle.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> SphereMaterialAsset(TEXT("/Script/Engine.Material'/Game/Shooting/BluePrint/Effect/Gun/RSkill/RangeSphereMaterial.RangeSphereMaterial'"));
	if (SphereMaterialAsset.Succeeded()) {
		SphereMaterial = SphereMaterialAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> CircleMaterialAsset(TEXT("/Script/Engine.Material'/Game/Shooting/BluePrint/Effect/Gun/RSkill/RangeCircleMaterial.RangeCircleMaterial'"));
	if (CircleMaterialAsset.Succeeded()) {
		CircleMaterial = CircleMaterialAsset.Object;
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem> SwapEffectAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Shooting/BluePrint/Effect/P_Wraith_Gun_Attach.P_Wraith_Gun_Attach'"));
	if (SwapEffectAsset.Succeeded()) {
		SwapEffect = SwapEffectAsset.Object;
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem> LaserEffectAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Shooting/BluePrint/Effect/P_Beam_Laser_Ice.P_Beam_Laser_Ice'"));
	if (LaserEffectAsset.Succeeded()) {
		LaserEffect = LaserEffectAsset.Object;
	}


	SetRSkill_Circle();

	AimTargetLength = 500.0f;
	MissleDistance = 500.0f;
}

void AGunCharacter::BeginPlay()
{
	Super::BeginPlay();


	if (Weapon) {
		WeaponGun = GetWorld()->SpawnActor<AWeaponGun>(Weapon);
		GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
		if (IsValid(WeaponGun)) {
			WeaponGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_r_WeaponSocket"));
			WeaponGun->SetActorRelativeLocation(FVector(-1.156438f, -9.918899f, -1.919678f));
			WeaponGun->SetOwner(this);
		}
	}
}

void AGunCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UGunAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance) {
		AnimInstance->AbilityREnd.AddUObject(this, &AGunCharacter::Attack_Skill_REnd);
	}
}

void AGunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BaseController) {
		
		SocketEndVector = FVector(GetActorForwardVector().X, GetActorForwardVector().Y,GetMesh()->GetSocketRotation("backpack_Socket").Vector().Z);
		SocketEndVector += SocketEndVector * LeftButtonDirection;
		
		TArray<FVector> OutPathResult;
		TArray<AActor*> IgnoreActors;
		TArray<TEnumAsByte<EObjectTypeQuery>> PredictObjectType;

		PredictObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

		bool bHit = UGameplayStatics::Blueprint_PredictProjectilePath_ByObjectType(
			this,
			OUT HitResult,
			OUT OutPathResult,
			OUT OutLastTraceDestinationResult,
			GetMesh()->GetSocketLocation("backpack_Socket"),
			SocketEndVector * MissleDistance,
			true,
			20.0f,
			PredictObjectType,
			false,
			IgnoreActors,
			EDrawDebugTrace::None,
			0.0f,
			10.0f,
			10.0f,
			0.0f
		);

		if (bHit) {
			auto NewLocation = FMath::VInterpTo(RSkillCircleMeshComponent->GetComponentLocation(), OutLastTraceDestinationResult, GetWorld()->GetDeltaSeconds(), 10.0f);
			RSkillCircleMeshComponent->SetWorldLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
		}
	}
		
	if (SpringArm) {
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, AimTargetLength, DeltaTime, 15.0f);
	}
}

void AGunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("LeftMouseAxis"), this, &AGunCharacter::RSkill_RangeDirection);

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AGunCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Q_Skill"), EInputEvent::IE_Pressed, this, &AGunCharacter::Attack_Skill_Q);
	PlayerInputComponent->BindAction(TEXT("R_Skill"), EInputEvent::IE_Pressed, this, &AGunCharacter::Attack_Skill_R);
	PlayerInputComponent->BindAction(TEXT("Aiming"), EInputEvent::IE_Pressed, this, &AGunCharacter::AimingStart);
	PlayerInputComponent->BindAction(TEXT("Aiming"), EInputEvent::IE_Released, this, &AGunCharacter::AimingEnd);
}



void AGunCharacter::Attack()
{
	if (ShouldAttack && !IsRunning) {
		IsAiming = true;
		WeaponGun->Shoot();
		
		if (GetWorld()->GetTimerManager().IsTimerActive(AimWaitHandle)) {
			GetWorld()->GetTimerManager().ClearTimer(AimWaitHandle);
		}

		GetWorld()->GetTimerManager().SetTimer(AimWaitHandle, FTimerDelegate::CreateLambda([&]() 
		{
			IsAiming = false;
		}), 10.0f, false);
	}
}

void AGunCharacter::Attack_Skill_Q()
{
	WeaponGun->SetActorHiddenInGame(true);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	GetMesh()->UnHideBoneByName(TEXT("weapon_r"));
	UParticleSystemComponent* SwapEffectComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SwapEffect, GetMesh()->GetSocketTransform("weapon_r"));
	if (SwapEffectComponent) {
		SwapEffectComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
		SwapEffectComponent->SetWorldLocation(SwapEffectComponent->GetComponentLocation() + GetActorRightVector() * -130.0f + GetActorForwardVector() *30.0f + GetActorUpVector() * -5.0f);
	}

	UParticleSystemComponent* LaserEffectComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaserEffect, GetMesh()->GetSocketTransform("gun_barrelSocket"));
	if (LaserEffectComponent) {
		LaserEffectComponent->CustomTimeDilation = 3.0f;
	}
	
	FHitResult LaserHitResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		OUT LaserHitResult,
		GetMesh()->GetSocketLocation("gun_barrelSocket"),
		GetMesh()->GetSocketLocation("gun_barrelSocket") + GetMesh()->GetSocketRotation("gun_barrelSocket").Vector() * 1000.0f,
		ECC_Visibility);

	DrawDebugLine(GetWorld(), GetMesh()->GetSocketLocation("gun_barrelSocket"),
		GetMesh()->GetSocketLocation("gun_barrelSocket") + GetMesh()->GetSocketRotation("gun_barrelSocket").Vector() * 1000.0f, FColor::Red, true, 2.0f);
	
}

void AGunCharacter::Attack_Skill_R()
{
	if (Should_R_Skill && !IsZooming && !IsAttacking_R_Skill) {
		IsAttacking_R_Skill = true;
		Should_R_Skill = false;
		ShouldAttack = false;
		ShouldMove = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		AnimInstance->PlayAttack_R_SkillMontage();
		RSkillCircleMeshComponent->SetVisibility(true);
		DecalComponent->SetVisibility(true);
	}
	else if (IsAttacking_R_Skill) {
		MissleLaunchVector = SocketEndVector + FVector(-1.5f, 0.0f, 0.0f);
		TargetLocation = OutLastTraceDestinationResult;
		UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
		AnimInstance->Montage_Resume(CurrentMontage);	
	}
}

void AGunCharacter::Attack_Skill_REnd()
{
	FTimerHandle waitHandle;
	GetWorld()->GetTimerManager().SetTimer(waitHandle, FTimerDelegate::CreateLambda([&]()
		{
			Should_R_Skill = true;
		}), 4.0f, false);

	for (int i = 0; i < 5; i++) {
		FVector CurrntMissleLaunchVector = MissleLaunchVector + FVector(0.5f * i,0.0f,0.0f);
		FTimerHandle LaunchTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(LaunchTimerHandle, FTimerDelegate::CreateLambda([CurrntMissleLaunchVector,this]()
			{
				auto aMissle = GetWorld()->SpawnActor<AMissle>(AMissle::StaticClass(), GetMesh()->GetSocketTransform("backpack_Socket"), FActorSpawnParameters());
				if (aMissle) {
					aMissle->ProjectileMovement->Velocity = CurrntMissleLaunchVector * 1000.0f;
					FTimerHandle MissleHandle;
					GetWorld()->GetTimerManager().SetTimer(MissleHandle, FTimerDelegate::CreateLambda([aMissle, this]()
						{
							aMissle->SetTarget(TargetLocation);
						}), 0.1f, false);
					MissleTimerHandles.Emplace(MissleHandle);
				}
			}), 0.1f + 0.1f * i, false);
	}

	FTimerHandle lHandle;
	GetWorld()->GetTimerManager().SetTimer(lHandle,	 FTimerDelegate::CreateLambda([&]()
		{
			LeftButtonDirection = 0.0f;
			MissleTimerHandles.Empty();
		}), 0.5f, false);

			
	
	RSkillCircleMeshComponent->SetVisibility(false);
	DecalComponent->SetVisibility(false);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	IsAttacking_R_Skill = false;
	ShouldAttack = true;
	ShouldMove = true;
}

void AGunCharacter::RSkill_RangeDirection(float value)
{
	LeftButtonDirection = FMath::FInterpTo(LeftButtonDirection, LeftButtonDirection + (value / 10), GetWorld()->GetDeltaSeconds(), 10.0f);
}



void AGunCharacter::AimingStart()
{
	if (!IsRunning) {
		IsAiming = true;
		IsZooming = true;
		ShouldRun = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		AimTargetLength = 200.f;
	}

}

void AGunCharacter::AimingEnd()
{
	if (!(GetWorld()->GetTimerManager().IsTimerActive(AimWaitHandle))) {
		IsAiming = false;
	}
	IsZooming = false;
	ShouldRun = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	AimTargetLength = 500.f;
}

void AGunCharacter::SetRSkill_Circle()
{
	RSkillCircleMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RSkillCircleMeshComponent"));
	RSkillCircleMeshComponent->SetupAttachment(GetCapsuleComponent());
	RSkillCircleMeshComponent->SetCollisionProfileName(TEXT("RangeCircle"));
	RSkillCircleMeshComponent->SetWorldScale3D(FVector(5.0f, 5.0f, 5.0f));
	RSkillCircleMeshComponent->SetStaticMesh(RSkillCircle);
	RSkillCircleMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	RSkillCircleMeshComponent->SetMaterial(0, SphereMaterial);
	RSkillCircleMeshComponent->SetVisibility(false);

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetMaterial(0, CircleMaterial);
	DecalComponent->SetupAttachment(RSkillCircleMeshComponent);
	DecalComponent->DecalSize = FVector(10.0f, 10.0f, 10.0f);
	DecalComponent->SetWorldScale3D(FVector(2.0f, 5.0f, 5.0f));
	DecalComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	DecalComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -10.0f));
	DecalComponent->SetVisibility(false);
}

float AGunCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float returnDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	

	return returnDamage;
}
