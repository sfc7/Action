// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicBase.generated.h"

UCLASS()
class SHOOTING_API AMagicBase : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* CollisionMesh;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere)
		class UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(VisibleAnywhere)
		class UParticleSystemComponent* ParticleSystem;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProjectileSettings, meta = (ClampMin = "0.0", ClampMax = "2.0", UIMin = "0.0", UIMax = "2.0",
		DisplayName = "Gravity", ExposeOnSpawn = "true"))
		float Gravity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProjectileSettings, meta = (DisplayName = "Speed"));
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProjectileSettings, meta = (DisplayName = "ImpactEffect", ExposeOnSpawn = "true"));
		UParticleSystem* ImpactEffect;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProjectileSettings, meta = (DisplayName = "TemplateEffect", ExposeOnSpawn = "true"));
		UParticleSystem* TemplateEffect;
public:	
	AMagicBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
