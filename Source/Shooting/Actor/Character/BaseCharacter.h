// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UMyGameInstance;

UCLASS()
class SHOOTING_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	UPROPERTY()
		UMyGameInstance* MyGameInstance;
	UPROPERTY(VisibleAnywhere)
		class UPlayerComponent* PlayerComponent;
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AActor> Weapon;
	UPROPERTY(VisibleAnywhere)
		class UCharacterUIWidget* CUI;
	UPROPERTY(VisibleAnywhere)
		class UParticleSystem* SpawnEffect;
	UPROPERTY(VisibleAnywhere)
		class USoundWave* AttackBasicSound;
	UPROPERTY(VisibleAnywhere)
		class USoundWave* AttackQSkillSound;
	UPROPERTY(VisibleAnywhere)
		class USoundWave* AttackRSkillSound;
	UPROPERTY(VisibleAnywhere)
		class USoundWave* HitSound;
	UPROPERTY(VisibleAnywhere)
		class USoundWave* DeathSound;
	UPROPERTY(VisibleAnywhere)
		class USoundWave* WeaponSound;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class APlayerController* BaseController;

public:
	ABaseCharacter();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
public:
	//Should
	bool ShouldMove = true;
	bool ShouldRun = true;
	bool ShouldAttack = true;
	bool ShouldJump = true;
	bool ShouldDodge = true;
	bool Should_Q_Skill = true;
	bool Should_R_Skill = true;
	bool IsAttacking;
	bool IsAttacking_Q_Skill;
	bool IsAttacking_R_Skill;
	bool IsDamaging;
	bool IsDodging;
	bool IsJumping;
	bool IsRunning;
	
	float Should_Q_Cooldown;
	float Should_R_Cooldown;
	//Combo
	bool attacking;
	bool CanNextCombo;
	bool IsComboInputOn;

	int CurrentCombo;
	int MaxCombo;

public:
	//Movement
	float WalkSpeed = 300.f;
	float SprintSpeed = 800.f;

	void MoveUpDown(float value);
	void MoveLeftRight(float value);
	void LookUpDown(float value);
	void LookLeftRight(float value);


	void SprintStart();
	void SprintEnd();

	//Dodge
	float MoveDistance = 500.0f;

	//ESkill_Range
	float LeftButtonDirection = 0.0f;
public:

	void ToggleDuringAttack(bool enable);
	void ToggleDuringHit(bool enable);
	void ToggleDuringDodge(bool enable);
	void ToggleAllAct();

	virtual void AttackStartComboState();
	virtual void AttackEndComboState();
	void JumpStart();

	void CharacterChangeMagic();
	void CharacterChangeKatana();
	void CharacterChangeGun();
	void CharacterChange(TSubclassOf<ABaseCharacter> ChangeCharacter);
	virtual void Death();
	
	void AttackBasicSoundPlay();
	void AttackRSkillSoundPlay();
	void AttackQSkillSoundPlay();
	void HitSoundPlay();
	void DeathSoundPlay();
	void WeaponSoundPlay();

};
