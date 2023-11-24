// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Missle.generated.h"

UCLASS()
class SHOOTING_API AMissle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMissle();
public:

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* CollisionMesh;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		class UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UParticleSystemComponent* ParticleSystem;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
		class URadialForceComponent* RadialForce;
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* HomingTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
		class UParticleSystem* ImpactEffect;
		UPROPERTY(EditAnywhere, BlueprintReadWrite);
		class UParticleSystem* SmokeEffect;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProjectileSettings, meta = (ClampMin = "0.0", ClampMax = "2.0", UIMin = "0.0", UIMax = "2.0",
		DisplayName = "Gravity", ExposeOnSpawn = "true"))
		float Gravity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProjectileSettings, meta = (DisplayName = "Speed"));
		float Speed;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void SetTarget(FVector& TargetLocation);

	UFUNCTION()
		void ProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};
