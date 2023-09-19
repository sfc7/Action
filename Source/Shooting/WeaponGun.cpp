// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GamePlayStatics.h"
#include "Particles/ParticleSystem.h"
#include "DrawDebugHelpers.h"
// Sets default values
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

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Flash(TEXT("/Script/Engine.ParticleSystem'/Game/Shooting/BluePrint/Effect/Gun/ParticleSystems/Muzzle/P_Wraith_Primary_MuzzleFlash.P_Wraith_Primary_MuzzleFlash'"));
	if (Flash.Succeeded()) {
		FlashEffect = Flash.Object;
	}
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

void AWeaponGun::Shoot()
{
	UGameplayStatics::SpawnEmitterAttached(FlashEffect, Skeletal, TEXT("MuzzleFlashSocket"));


	APawn* Shooter = Cast<APawn>(GetOwner());
	if (IsValid(Shooter)) {
		auto ShooterController = Shooter->GetController();
		if (IsValid(ShooterController)) {
			FVector ControllerLocation = GetActorLocation();
			FRotator ControllerRotation = GetActorRotation();
			ShooterController->GetPlayerViewPoint(OUT ControllerLocation, OUT ControllerRotation);

			DrawDebugCamera(GetWorld(), ControllerLocation, ControllerRotation, 90, 2, FColor::Red, true);
		}
	}

	


}


