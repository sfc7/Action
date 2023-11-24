// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicBall.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMagicBall::AMagicBall()
{
	PrimaryActorTick.bCanEverTick = true;

	Speed = 2000.f;
	Gravity = 0.f;

	ConstructorHelpers::FObjectFinder<UStaticMesh> magic(TEXT("/Script/Engine.StaticMesh'/Game/ParagonGideon/FX/Meshes/Heroes/Gideon/SM_Black_Hole_Main.SM_Black_Hole_Main'"));

	if (magic.Succeeded()) {
		StaticMesh->SetStaticMesh(magic.Object);
	}

	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
	StaticMesh->SetRelativeScale3D(FVector(0.5, 0.5f, 0.5f));

	CollisionMesh->SetBoxExtent(FVector(25.0f, 25.0f, 25.0f));
	CollisionMesh->SetCollisionProfileName(TEXT("MagicBall"));

	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
	ProjectileMovement->ProjectileGravityScale = Gravity;
	ProjectileMovement->HomingAccelerationMagnitude = 40000.0f;
	
	ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonGideon/FX/Particles/Gideon/Abilities/Burden/FX/P_Gideon_Burden_HitCharacter.P_Gideon_Burden_HitCharacter'"));
	if (ParticleAsset.Succeeded()) {
		ImpactEffect = ParticleAsset.Object;
	}
	
}


void AMagicBall::BeginPlay()
{
	Super::BeginPlay();
	
	//생성자에서는 작용하지 않음
	CollisionMesh->OnComponentHit.AddDynamic(this, &AMagicBall::ProjectileImpact);
}

void AMagicBall::ProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComponent != nullptr) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint);
		UGameplayStatics::ApplyDamage(Target, 5.0f, GetInstigatorController(), Target, NULL);
		Destroy();
	}
}


void AMagicBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMagicBall::RotateToTarget()
{
	ProjectileMovement->Velocity = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), Target->GetActorLocation()) * Speed;
	
}

void AMagicBall::SetTarget(AActor* _target)
{
	Target = _target;

	if (IsValid(Target)) {
		RotateToTarget();
		ProjectileMovement->bIsHomingProjectile = true;
		ProjectileMovement->HomingTargetComponent = Target->GetRootComponent();
	}
}

