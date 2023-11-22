// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatureComponent.h"
#include "kismet/GameplayStatics.h"


UCreatureComponent::UCreatureComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}


void UCreatureComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetInitialize();
}


void UCreatureComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCreatureComponent::SetInitialize()
{
	
}

void UCreatureComponent::SetHp(int32 _Hp)
{

}

void UCreatureComponent::OnDamaged(float Damage)
{


}

float UCreatureComponent::GetHpRatio()
{
	if (Hp == 0) {
		return 0.f;
	}

	return (float)Hp / (float)MaxHp;
}


