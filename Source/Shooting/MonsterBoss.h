// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseMonster.h"
#include "MonsterBoss.generated.h"


/**
 * 
 */
UCLASS()
class SHOOTING_API AMonsterBoss : public ABaseMonster
{
	GENERATED_BODY()
public:
	AMonsterBoss();

	UPROPERTY(VisibleAnywhere)
		class UParticleSystem* TeleportBodyEffect;
	UPROPERTY(VisibleAnywhere)
		class UParticleSystem* TeleportTrailEffect;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UParticleSystemComponent* TeleportBodyComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UParticleSystemComponent* TeleportTrailComponent;


public:
	FString Stance;
protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	void Attack(AActor* Target);

	void Teleport(FVector _Location);
	void TeleportEnd();
};
