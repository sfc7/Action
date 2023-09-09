// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicStorm.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

AMagicStorm::AMagicStorm()
{
	Speed = 500.0f;
	Gravity = 0.0f;
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));

	CollisionMesh->SetBoxExtent(FVector(150.0f, 150.0f, 70.0f));
	CollisionMesh->SetCollisionProfileName(TEXT("MagicStorm"));

	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
	ProjectileMovement->ProjectileGravityScale = Gravity;

	ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_darkStorm.P_ky_darkStorm'"));
	if (ParticleAsset.Succeeded()) {
		ParticleSystem->SetTemplate(ParticleAsset.Object);
	}


}

void AMagicStorm::BeginPlay()
{
	Super::BeginPlay();

	CollisionMesh->OnComponentHit.AddDynamic(this, &AMagicStorm::ProjectileImpact);
	CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &AMagicStorm::ProjectileOverlap);

	FTimerHandle waitHandle;
	GetWorld()->GetTimerManager().SetTimer(waitHandle, FTimerDelegate::CreateLambda([&]()
	{
		Destroy();
	}), 3.0f, false);
}

void AMagicStorm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMagicStorm::ProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}

void AMagicStorm::ProjectileOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherIndex, bool bFromSweep, const FHitResult& Hit)
{

	FTimerHandle waitHandle;
	if (OtherActor != nullptr && OtherActor != this && OtherComponent != nullptr) {
		if (Hit.GetActor()->GetClass()->GetSuperClass()->GetSuperClass()->GetName() == TEXT("BaseMonster")) {
			UGameplayStatics::ApplyDamage(Hit.GetActor(), 15.0f, GetInstigatorController(), Hit.GetActor(), NULL);
		}
	}

}


