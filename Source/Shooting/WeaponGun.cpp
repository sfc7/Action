// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponGun.h"
#include "Components/SkeletalMeshComponent.h"


// Sets default values
AWeaponGun::AWeaponGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Skeletal = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	Skeletal->SetupAttachment(Root);



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

