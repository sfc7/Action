// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GamePlayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundWave.h"
#include "Sound/SoundCue.h"
#include "DrawDebugHelpers.h"
#include "Engine/DamageEvents.h"

AWeaponGun::AWeaponGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Skeletal = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	Skeletal->SetupAttachment(Root);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Gun(TEXT("/Script/Engine.SkeletalMesh'/Game/Shooting/BluePrint/Mesh/Weapon/Gun/Rifle.Rifle'"));
	if (Gun.Succeeded()) {
		Skeletal->SetSkeletalMesh(Gun.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Flash(TEXT("/Script/Engine.ParticleSystem'/Game/Shooting/BluePrint/Effect/P_Wraith_Primary_MuzzleFlash.P_Wraith_Primary_MuzzleFlash'"));
	if (Flash.Succeeded()) {
		FlashEffect = Flash.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Impact(TEXT("/Script/Engine.ParticleSystem'/Game/Shooting/BluePrint/Effect/P_AssaultRifle_IH.P_AssaultRifle_IH'"));
	if (Impact.Succeeded()) {
		ImpactEffect = Impact.Object;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> LaserImpact(TEXT("/Script/Engine.ParticleSystem'/Game/Shooting/BluePrint/Effect/P_AssaultRifle_IH.P_AssaultRifle_IH'"));
	if (LaserImpact.Succeeded()) {
		LaserImpactEffect = LaserImpact.Object;
	}

	static  ConstructorHelpers::FObjectFinder<USoundCue> GunSoundAsset(TEXT("/Script/Engine.SoundCue'/Game/Shooting/BluePrint/Sound/Character/Gun/SC_AssaultRifle_Shot.SC_AssaultRifle_Shot'"));
	if (GunSoundAsset.Succeeded()) {
		GunSound = GunSoundAsset.Object;
	}

	static  ConstructorHelpers::FObjectFinder<USoundCue> ImpactSoundAsset(TEXT("/Script/Engine.SoundCue'/Game/Shooting/BluePrint/Sound/Character/Gun/SC_AssaultRifle_Impact.SC_AssaultRifle_Impact'"));
	if (ImpactSoundAsset.Succeeded()) {
		ImpactSound = ImpactSoundAsset.Object;
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem> LaserEffectAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Shooting/BluePrint/Effect/P_Beam_Laser_Ice.P_Beam_Laser_Ice'"));
	if (LaserEffectAsset.Succeeded()) {
		LaserEffect = LaserEffectAsset.Object;
	}

	Range = 2000.0f;
}

// Called when the game starts or when spawned
void AWeaponGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
}

void AWeaponGun::GunSoundPlay()
{
	
}

void AWeaponGun::Shoot(FTransform StartTransForm)
{
	UGameplayStatics::SpawnEmitterAttached(FlashEffect, Skeletal, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(GunSound, Skeletal, TEXT("GunSound"));

	APawn* Shooter = Cast<APawn>(GetOwner());
	if (IsValid(Shooter)) {
		auto ShooterController = Shooter->GetController(); 
		if (IsValid(ShooterController)) {
			StartTransForm.GetLocation();
			FVector End = StartTransForm.GetLocation() + StartTransForm.GetRotation().Vector() * Range;

			bool bHit;
			FHitResult HitResult;
			FCollisionQueryParams Params(NAME_None, false, this);

			bHit = GetWorld()->LineTraceSingleByChannel(
				OUT HitResult,
				StartTransForm.GetLocation(),
				End,
				ECollisionChannel::ECC_GameTraceChannel10,
				Params
			);

			DrawDebugLine(GetWorld(), StartTransForm.GetLocation(), End, FColor::Red, true, 2.0f);
			if (bHit) {
				FVector Dir = -StartTransForm.GetRotation().Vector();
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.Location, Dir.Rotation());
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, HitResult.Location);
				if (HitResult.GetActor() != nullptr) {
					float Damage = 2.0f;
					FPointDamageEvent DamageEvent(Damage, HitResult, Dir, nullptr);
					HitResult.GetActor()->TakeDamage(Damage, DamageEvent, ShooterController, this);
				}
			}
		}
	}
}

void AWeaponGun::ZoomShoot()
{
	UGameplayStatics::SpawnEmitterAttached(FlashEffect, Skeletal, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(GunSound, Skeletal, TEXT("GunSound"));

	APawn* Shooter = Cast<APawn>(GetOwner());
	if (IsValid(Shooter)) {
		auto ShooterController = Shooter->GetController();
		if (IsValid(ShooterController)) {

			FVector ControllerLocation;
			FRotator ControllerRotation;
			ShooterController->GetPlayerViewPoint(OUT ControllerLocation, OUT ControllerRotation);

			FVector End = ControllerLocation + ControllerRotation.Vector() * Range;
			
			bool bHit;
			FHitResult HitResult;
			FCollisionQueryParams Params(NAME_None, false, this);

			bHit = GetWorld()->LineTraceSingleByChannel(
				OUT HitResult,
				ControllerLocation,
				End,
				ECollisionChannel::ECC_GameTraceChannel10,
				Params
			);

			DrawDebugLine(GetWorld(), ControllerLocation, End, FColor::Red, true, 2.0f);
			if (bHit) {
				FVector Dir = -ControllerRotation.Vector();
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactEffect, HitResult.Location, Dir.Rotation());
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, HitResult.Location);
				if (HitResult.GetActor() != nullptr) {
					float Damage = 2.0f;
					FPointDamageEvent DamageEvent(Damage, HitResult, Dir, nullptr);
					HitResult.GetActor()->TakeDamage(Damage, DamageEvent, ShooterController, this);
				}
			}
		}
	}
}

void AWeaponGun::LaserShoot(FTransform StartTransForm)
{
	APawn* Shooter = Cast<APawn>(GetOwner());
	if (IsValid(Shooter)) {
		auto ShooterController = Shooter->GetController();
		if (IsValid(ShooterController)) {

			FVector ControllerLocation;
			FRotator ControllerRotation;
			ShooterController->GetPlayerViewPoint(OUT ControllerLocation, OUT ControllerRotation);

			FVector Direction = (ControllerLocation + ControllerRotation.Vector() * Range) - StartTransForm.GetLocation();
			Direction.Normalize();

			FVector End = ControllerLocation + Direction * Range;

			bool bHit;
			FHitResult HitResult;
			FCollisionQueryParams Params(NAME_None, false, this);

			bHit = GetWorld()->LineTraceSingleByChannel(
				OUT HitResult,
				StartTransForm.GetLocation(),
				End,
				ECollisionChannel::ECC_GameTraceChannel3,
				Params
			);

			UParticleSystemComponent* LaserEffectComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaserEffect, StartTransForm.GetLocation(), (End - StartTransForm.GetLocation()).Rotation());
			if (LaserEffectComponent) {
				LaserEffectComponent->CustomTimeDilation = 3.0f;
			}

			DrawDebugLine(GetWorld(), StartTransForm.GetLocation(), End, FColor::Red, true, 2.0f);

			if (bHit) {
				FVector Dir = -ControllerRotation.Vector();
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaserImpactEffect, HitResult.Location, Dir.Rotation());
				if (HitResult.GetActor() != nullptr) {
					float Damage = 20.0f;
					FPointDamageEvent DamageEvent(Damage, HitResult, Dir, nullptr);
					HitResult.GetActor()->TakeDamage(Damage, DamageEvent, ShooterController, this);
				}
			}
		}
	}
}


