// Fill out your copyright notice in the Description page of Project Settings.


#include "RespawnLocation.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ARespawnLocation::ARespawnLocation()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComp);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(BoxComp);

	BoxComp->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void ARespawnLocation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARespawnLocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARespawnLocation::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

