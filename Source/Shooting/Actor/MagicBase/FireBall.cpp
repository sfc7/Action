// Fill out your copyright notice in the Description page of Project Settings.


#include "FireBall.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AFireBall::AFireBall()
{
	PrimaryActorTick.bCanEverTick = true;

	Speed = 2000.f;
	Gravity = 0.f;


	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));

	CollisionMesh->SetBoxExtent(FVector(25.0f, 25.0f, 25.0f));
	CollisionMesh->SetCollisionProfileName(TEXT("FireBall"));

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



void AFireBall::BeginPlay()
{
	Super::BeginPlay();

	CollisionMesh->OnComponentHit.AddDynamic(this, &AFireBall::ProjectileImpact);
}

void AFireBall::ProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComponent != nullptr) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint);
		if (Hit.GetActor()->GetClass()->GetSuperClass()->GetName() == TEXT("BaseCharacter")) {
			UGameplayStatics::ApplyDamage(Hit.GetActor(), 20.0f, GetInstigatorController(), Hit.GetActor(), NULL);
		}
		Destroy();
	}
}

void AFireBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFireBall::RotateToTarget()
{
	ProjectileMovement->Velocity = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), TargetLocation) * Speed;
}

void AFireBall::SetTargetLocation(FVector _TargetLocation)
{
	TargetLocation = _TargetLocation;
	RotateToTarget();
}
