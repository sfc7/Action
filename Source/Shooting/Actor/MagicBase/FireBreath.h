// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MagicBase.h"
#include "FireBreath.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API AFireBreath : public AMagicBase
{
	GENERATED_BODY()
public:
	AFireBreath();
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
		void ProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
		void ProjectileOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherIndex, bool bFromSweep, const FHitResult& Hit);
public:
	virtual void Tick(float DeltaTime) override;
public:
	FVector TargetLocation;

	void RotateToTarget();
	void SetTargetLocation(FVector _TargetLocation);
};
