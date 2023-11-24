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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UParticleSystem* TeleportBodyEffect;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UParticleSystem* TeleportTrailEffect;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UParticleSystem* DashAttackBodyEffect;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UParticleSystem* DashAttackTrailEffect;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UParticleSystem* GatePortalEffect;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UParticleSystemComponent* TeleportBodyComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UParticleSystemComponent* TeleportTrailComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UParticleSystemComponent* DashAttackBodyComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UParticleSystemComponent* DashAttackTrailComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<UArrowComponent*> ArrowComponents;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor> WeaponClass;
	UPROPERTY(EditAnywhere)
		AActor* Weapon;
	UPROPERTY(VisibleAnywhere)
		class UParticleSystem* TakeHitEffect;
	UPROPERTY(VisibleAnywhere)
		class UMonsterWidget* MUI;


public:
	FVector TargetLocation;
	FString Stance;
	float CompleteHeal = 0.0f;
	float HealFrame = 2.0f;
	FTimerHandle HealHandle;
protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:
	void Attack(AActor* Target);
	void Attack_Basic(float damage);
	void RangeAttack(AActor* Target);
	void Spawn_Fireball();
	void Spawn_MultipleFireball();
	void Spawn_GateofBabylon();
	void DashAttack(AActor* Target);
	void Heal();
	void Teleport(FVector _Location);
	void TeleportEnd();
	void SpawnWeapon_l();
	void SpawnWeapon_r();
	void SpawnWeapon_spine();

};
