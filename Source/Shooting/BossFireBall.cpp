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

	Speed = 1500.f;
	Gravity = 0.f;


	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));

	CollisionMesh->SetBoxExtent(FVector(25.0f, 25.0f, 25.0f));
	CollisionMesh->SetCollisionProfileName(TEXT("FireBall"));

	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
	ProjectileMovement->ProjectileGravityScale = Gravity;

	ConstructorHelpers::FObjectFinder<UParticleSystem> ImpactParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Shooting/BluePrint/Effect/P_FireBall_Powerup.P_FireBall_Powerup'"));
	if (ImpactParticleAsset.Succeeded()) {
		ImpactEffect = ImpactParticleAsset.Object;
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem> TemplateParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Shooting/BluePrint/Effect/P_FireBall_Strong.P_FireBall_Strong'"));
	if (TemplateParticleAsset.Succeeded()) {
		TemplateEffect = TemplateParticleAsset.Object;
	}

	ParticleSystem->SetTemplate(TemplateEffect);
	ParticleSystem->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
}

void ABossFireBall::BeginPlay()
{
	Super::BeginPlay();

	CollisionMesh->OnComponentHit.AddDynamic(this, &ABossFireBall::ProjectileImpact);
}

void ABossFireBall::ProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComponent != nullptr) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint);
		if (Hit.GetActor()->GetClass()->GetSuperClass()->GetName() == TEXT("BaseCharacter")) {
			UGameplayStatics::ApplyDamage(Hit.GetActor(), 20.0f, GetInstigatorController(), Hit.GetActor(), NULL);
		}
		Destroy();
	}
}

void ABossFireBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABossFireBall::RotateToTarget()
{
	ProjectileMovement->Velocity = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), TargetLocation) * Speed;
}

void ABossFireBall::SetTargetLocation(FVector _TargetLocation)
{
	TargetLocation = _TargetLocation;
	RotateToTarget();	
}
