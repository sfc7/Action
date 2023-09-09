// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnLocation_Stage1_Event1.h"
#include "Components/ArrowComponent.h"

// Sets default values
ARespawnLocation_Stage1_Event1::ARespawnLocation_Stage1_Event1()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	SetRootComponent(Arrow);
}

// Called when the game starts or when spawned
void ARespawnLocation_Stage1_Event1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARespawnLocation_Stage1_Event1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARespawnLocation_Stage1_Event1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

