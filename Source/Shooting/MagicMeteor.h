// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MagicBase.h"
#include "MagicMeteor.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTING_API AMagicMeteor : public AMagicBase
{
	GENERATED_BODY()
public:
	AMagicMeteor();
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void ProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

};
