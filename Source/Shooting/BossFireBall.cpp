// Fill out your copyright notice in the Description page of Project Settings.


#include "BossFireBall.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ABossFireBall::ABossFireBall()
{
	PrimaryActorTick.bCanEverTick = true;

	Speed = 2000.f;
	Gravity = 0.f;

	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));

	CollisionMesh->SetBoxExtent(FVector(25.0f, 25.0f, 25.0f));
	CollisionMesh->SetCollisionProfileName(TEXT("BossFireBall"));

	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
	ProjectileMovement->ProjectileGravityScale = Gravity;

	ConstructorHelpers::FObjectFinder<UParticleSystem> ImpactParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_explosion.P_ky_explosion'"));
	if (ImpactParticleAsset.Succeeded()) {
		ImpactEffect = ImpactParticleAsset.Object;
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem> TemplateParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_fireBall.P_ky_fireBall'"));
	if (TemplateParticleAsset.Succeeded()) {
		TemplateEffect = TemplateParticleAsset.Object;
	}

	ParticleSystem->SetTemplate(TemplateEffect);
	ParticleSystem->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
}

void ABossFireBall::BeginPlay()
{
}

void ABossFireBall::ProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
}

void ABossFireBall::Tick(float DeltaTime)
{
}

void ABossFireBall::RotateToTarget()
{
}

void ABossFireBall::SetTargetLocation(FVector _TargetLocation)
{
}
