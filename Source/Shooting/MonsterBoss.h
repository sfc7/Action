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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<UArrowComponent*> ArrowComponents;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor> Weapon;
	UPROPERTY(VisibleAnywhere)
		class UParticleSystem* TakeHitEffect;


public:
	FVector TargetLocation;
	FString Stance;
protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
public:
	void Attack(AActor* Target);
	void Attack_Basic(float damage);
	void RangeAttack(AActor* Target);
	void Spawn_Fireball();
	void Spawn_MultipleFireball();

	void SetArrowComponent();
	void Teleport(FVector _Location);
	void TeleportEnd();
};
