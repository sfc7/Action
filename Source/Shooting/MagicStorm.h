// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MagicBase.h"
#include "MagicStorm.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API AMagicStorm : public AMagicBase
{
	GENERATED_BODY()
public:
	AMagicStorm();
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void ProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
		void ProjectileOverlap(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherIndex, bool bFromSweep, const FHitResult& Hit);
};
