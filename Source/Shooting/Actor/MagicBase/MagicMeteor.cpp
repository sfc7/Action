// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicMeteor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


AMagicMeteor::AMagicMeteor()
{
	PrimaryActorTick.bCanEverTick = true;

	Speed = 0.0f;
	Gravity = 2.0f;

	ConstructorHelpers::FObjectFinder<UStaticMesh> magic(TEXT("/Script/Engine.StaticMesh'/Game/ParagonGideon/FX/Meshes/Heroes/Gideon/Abilities/SM_MeteorChopped_Meteor_Chopped8.SM_MeteorChopped_Meteor_Chopped8'"));

	if (magic.Succeeded()) {
		StaticMesh->SetStaticMesh(magic.Object);
	}
	
	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
	StaticMesh->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));

	CollisionMesh->SetBoxExtent(FVector(150.0f, 150.0f, 70.0f));
	CollisionMesh->SetCollisionProfileName(TEXT("MagicBall"));

	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
	ProjectileMovement->ProjectileGravityScale = Gravity;

	ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/ProjectileMeteor/FX/Proto/Proto_P_Gideon_MeteorImpact.Proto_P_Gideon_MeteorImpact'"));
	if (ParticleAsset.Succeeded()) {
		ImpactEffect = ParticleAsset.Object;
	}
	
}

void AMagicMeteor::BeginPlay()
{
	Super::BeginPlay();

	CollisionMesh->OnComponentHit.AddDynamic(this, &AMagicMeteor::ProjectileImpact);
}

void AMagicMeteor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMagicMeteor::ProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComponent != nullptr) {
		auto CurrentImpact = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint);
		CurrentImpact->SetRelativeScale3D(FVector(0.3f,0.3f, 0.3f));
		if (Hit.GetActor()->GetClass()->GetSuperClass()->GetName() == TEXT("BaseMonster")) {
			UGameplayStatics::ApplyDamage(Hit.GetActor(), 15.0f, GetInstigatorController(), Hit.GetActor(), NULL);
		}
	
		Destroy();
	}
}
