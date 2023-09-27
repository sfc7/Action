// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponGun.generated.h"

UCLASS()
class SHOOTING_API AWeaponGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* Skeletal;
	UPROPERTY(EditAnywhere)
		class UParticleSystem* FlashEffect;
	UPROPERTY(EditAnywhere)
		class UParticleSystem* ImpactEffect;
	UPROPERTY(EditAnywhere)
		class USoundCue* GunSound;
	UPROPERTY(EditAnywhere)
		class USoundCue* ImpactSound;
public:

	void GunSoundPlay();
	float Range;
	void Shoot();

};
