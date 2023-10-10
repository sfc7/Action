// Fill out your copyright notice in the Description page of Project Settings.


#include "GunCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Math/UnrealMathUtility.h"
#include "GunAnimInstance.h"
#include "Kismet/GameplayStatics.h"
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
	


	static ConstructorHelpers::FObjectFinder<UStaticMesh> SkillCircle(TEXT("/Script/Engine.StaticMesh'/Game/Shooting/BluePrint/Effect/Gun/Q_SkillCircle.Q_SkillCircle'"));
	if (SkillCircle.Succeeded()) {
		RSkillCircle = SkillCircle.Object;
	}


	RSkillCircleMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RSkillCircleMeshComponent"));
	RSkillCircleMeshComponent->SetupAttachment(GetCapsuleComponent());
	RSkillCircleMeshComponent->SetCollisionProfileName(TEXT("RangeCircle"));


	AimTargetLength = 500.0f;
}

void AGunCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (RSkillCircleMeshComponent) {
		RSkillCircleMeshComponent->SetStaticMesh(RSkillCircle);
		RSkillCircleMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	}

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
		
		FVector SocketEndVector = GetActorForwardVector();
		SocketEndVector += SocketEndVector * LeftButtonDirection;
		
		FHitResult HitResult;
		TArray<FVector> OutPathResult;
		FVector OutLastTraceDestinationResult;
		TArray<AActor*> IgnoreActors;
		TArray<TEnumAsByte<EObjectTypeQuery>> PredictObjectType;

		PredictObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

		bool bHit = UGameplayStatics::Blueprint_PredictProjectilePath_ByObjectType(
			this,
			HitResult,
			OutPathResult,
			OutLastTraceDestinationResult,
			GetMesh()->GetSocketLocation("backpack_Socket"),
			SocketEndVector * 1000.0f,
			true,
			20.0f,
			PredictObjectType,
			false,
			IgnoreActors,
			EDrawDebugTrace::ForDuration,
			0.0f,
			10.0f,
			10.0f,
			0.0f
		);

		RSkillCircleMeshComponent->SetWorldLocation(OutLastTraceDestinationResult,false,nullptr,ETeleportType::TeleportPhysics);
		
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
	}
	else if (IsAttacking_R_Skill) {
		UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
		AnimInstance->Montage_Resume(CurrentMontage);
	}

}

void AGunCharacter::Attack_Skill_REnd()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	IsAttacking_R_Skill = false;
	Should_R_Skill = true;
	ShouldAttack = true;
	ShouldMove = true;
	
}

void AGunCharacter::RSkill_RangeDirection(float value)
{
	LeftButtonDirection = FMath::FInterpTo(LeftButtonDirection, LeftButtonDirection + (value / 20), GetWorld()->GetDeltaSeconds(), 10.0f);;
	
}



void AGunCharacter::AimingStart()
{
	if (!IsRunning) {
		IsAiming = true;
		IsZooming = true;
		ShouldRun = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		AimTargetLength = 100.f;
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

float AGunCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float returnDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	

	return returnDamage;
}
