// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "GunCharacter.generated.h"

class AWeaponGun;
class UGunAnimInstance;
class AMissle;
/**
 * 
 */
UCLASS()
class SHOOTING_API AGunCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float AimTargetLength;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float MissleDistance;
	UPROPERTY()
		FVector SocketEndVector;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector OutLastTraceDestinationResult;
	UPROPERTY()
		FVector MissleVelocity;
	UPROPERTY(VisibleAnywhere)
		UGunAnimInstance* AnimInstance;
	UPROPERTY()
		AWeaponGun* WeaponGun;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* RSkillCircleMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMesh* RSkillCircle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UDecalComponent* DecalComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UMaterial* CircleMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UMaterial* SphereMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UParticleSystem* SwapEffect;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UParticleSystem* LaserEffect;

public:
	AGunCharacter();
protected:
	virtual void BeginPlay() override;
public:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	FTimerHandle AimWaitHandle;
	FTimerHandle FireHandle;
	TArray<FTimerHandle> LaunchTimerHandles;
	TArray<FTimerHandle> MissleTimerHandles;
	bool IsAiming;
	bool IsZooming;
	bool IsAimWaving;
	FHitResult HitResult;
	FVector TargetLocation;
	FVector MissleLaunchVector;


	void Attack();
	void Attack_Skill_Q();
	void Attack_Skill_R();
	void Attack_Skill_REnd();
	void RSkill_RangeDirection(float value);
	void AimingStart();
	void AimingEnd();
	void SetRSkill_Circle();
	void StopFire();

	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
