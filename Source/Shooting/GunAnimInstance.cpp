// Fill out your copyright notice in the Description page of Project Settings.


#include "GunAnimInstance.h"
#include "GunCharacter.h"

void UGunAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	if(IsValid(Creature)) {
		GunCreature = Cast<AGunCharacter>(Creature);
	}
	
}

void UGunAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(GunCreature)) {
		IsAiming = GunCreature->IsAiming;
	}
}
