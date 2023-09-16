// Fill out your copyright notice in the Description page of Project Settings.


#include "GunCharacter.h"

AGunCharacter::AGunCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Skeletal(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonWraith/Characters/Heroes/Wraith/Meshes/Wraith.Wraith'"));

	if (Skeletal.Succeeded()) {
		GetMesh()->SetSkeletalMesh(Skeletal.Object);
	}
}

void AGunCharacter::BeginPlay()
{
	Super::BeginPlay();
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

}
