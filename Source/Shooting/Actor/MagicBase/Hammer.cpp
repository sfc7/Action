// Fill out your copyright notice in the Description page of Project Settings.


#include "Hammer.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AHammer::AHammer()
{
	PrimaryActorTick.bCanEverTick = true;

	Speed = 500.f;
	Gravity = 0.f;


	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));

	CollisionMesh->SetBoxExtent(FVector(25.0f, 25.0f, 25.0f));
	CollisionMesh->SetCollisionProfileName(TEXT("FireBall"));

	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
	ProjectileMovement->ProjectileGravityScale = Gravity;

	ConstructorHelpers::FObjectFinder<UParticleSystem> ImpactParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Shooting/BluePrint/Effect/P_Impact_Player_Fire_Weak.P_Impact_Player_Fire_Weak'"));
	if (ImpactParticleAsset.Succeeded()) {
		ImpactEffect = ImpactParticleAsset.Object;
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem> TemplateParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/Shooting/BluePrint/Effect/P_Skill_Throw_Base_Proj_Hammer_Force.P_Skill_Throw_Base_Proj_Hammer_Force'"));
	if (TemplateParticleAsset.Succeeded()) {
		TemplateEffect = TemplateParticleAsset.Object;
	}

	ParticleSystem->SetTemplate(TemplateEffect);
	ParticleSystem->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	ParticleSystem->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
}

void AHammer::BeginPlay()
{
	Super::BeginPlay();

	CollisionMesh->OnComponentHit.AddDynamic(this, &AHammer::ProjectileImpact);
}

void AHammer::ProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComponent != nullptr) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint);
		if (Hit.GetActor()->GetClass()->GetSuperClass()->GetName() == TEXT("BaseCharacter")) {
			UGameplayStatics::ApplyDamage(Hit.GetActor(), 2.0f, GetInstigatorController(), Hit.GetActor(), NULL);
		}
		Destroy();
	}
}

void AHammer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHammer::RotateToTarget()
{
	ProjectileMovement->Velocity = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), TargetLocation) * Speed;
}

void AHammer::SetTargetLocation(FVector _TargetLocation)
{
	TargetLocation = _TargetLocation;
	RotateToTarget();
}
