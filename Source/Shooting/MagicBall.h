// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MagicBase.h"
#include "MagicBall.generated.h"

UCLASS()
class SHOOTING_API AMagicBall : public AMagicBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProjectileSettings, meta = (DisplayName = "Target"))
		AActor* Target;
public:
	AMagicBall();
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
		void ProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
public:	
	virtual void Tick(float DeltaTime) override;
public:
	void RotateToTarget();
	void SetTarget(AActor* _target);
};
