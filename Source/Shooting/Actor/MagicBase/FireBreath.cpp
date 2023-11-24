// Fill out your copyright notice in the Description page of Project Settings.


#include "FireBreath.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AFireBreath::AFireBreath()
{
	PrimaryActorTick.bCanEverTick = true;

	Speed = 1000.0f;
	Gravity = 0.0f;

	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));

	CollisionMesh->SetBoxExtent(FVector(100.0f, 200.0f, 100.0f));
	CollisionMesh->SetCollisionProfileName(TEXT("FireBreath"));

	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
	ProjectileMovement->ProjectileGravityScale = Gravity;


	ConstructorHelpers::FObjectFinder<UParticleSystem> TemplateParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_fireBall.P_ky_fireBall'"));
	if (TemplateParticleAsset.Succeeded()) {
		TemplateEffect = TemplateParticleAsset.Object;
	}

	ParticleSystem->SetTemplate(TemplateEffect);
	ParticleSystem->SetRelativeScale3D(FVector(10.0f, 10.0f, 2.0f));
}

void AFireBreath::BeginPlay()
{
	Super::BeginPlay();

	CollisionMesh->OnComponentHit.AddDynamic(this, &AFireBreath::ProjectileImpact);
	CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &AFireBreath::ProjectileOverlap);

	FTimerHandle waitHandle;
	GetWorld()->GetTimerManager().SetTimer(waitHandle, FTimerDelegate::CreateLambda([&]()
		{
			Destroy();
		}), 1.0f, false);
}

void AFireBreath::ProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}

void AFireBreath::ProjectileOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComponent != nullptr) {
		if (Hit.GetActor()->GetClass()->GetSuperClass()->GetName() == TEXT("BaseCharacter")) {
			UGameplayStatics::ApplyDamage(Hit.GetActor(), 5.0f, GetInstigatorController(), Hit.GetActor(), NULL);
		}
	}
}

void AFireBreath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFireBreath::RotateToTarget()
{
	ProjectileMovement->Velocity = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), TargetLocation - FVector(0.0f,0.0f,-100.0f)) * Speed;
}

void AFireBreath::SetTargetLocation(FVector _TargetLocation)
{
	TargetLocation = _TargetLocation;
	RotateToTarget();
}
