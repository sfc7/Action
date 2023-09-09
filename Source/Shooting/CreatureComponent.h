// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CreatureComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpChanged);
DECLARE_MULTICAST_DELEGATE(FOnDeath);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTING_API UCreatureComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = CharacterInfo)
		int32 MaxHp;
	UPROPERTY(EditAnywhere, Category = CharacterInfo)
		int32 Hp;

public:
	FOnHpChanged OnHpChanged;
	FOnDeath OnDeath;

public:	
	// Sets default values for this component's properties
	UCreatureComponent();

protected:
	// Called when the game starts
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;

public:	
	class UMyGameInstance* GameInstance;

	virtual void SetInitialize();
	virtual void SetHp(int32 _Hp);
	virtual void OnDamaged(float Damage);
	int32 GetHp() { return Hp; }
	float GetHpRatio();

		
};
