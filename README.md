# Action
- 사용엔진 Unreal 5.2
  
- 장르 : 3인칭 액션

시연 영상:
https://www.youtube.com/watch?v=mSPkphVwWP4

진행과정 및 내용은 진행과정기술서(PDF) 폴더에 서술하였습니다. 
   
# 조작
W,A,S,D (이동)  
  
마우스 좌클릭 (공격)  
  
마우스 우클릭 (조준 - GunCharacter)  
  
Q,R (스킬사용)  
  
E (회피)  

Shift (달리기)  
  
SPACE (점프)  
  
F1, F2, F3 (캐릭터 전환)  
   

# 기술 설명서
## 구조 다이어그램
![Diagram](https://github.com/sfc7/Action/assets/124154607/e97dcd46-717a-4c77-9a48-6f0e8cc26091)
   
## 캐릭터
### 공통
- 캐릭터는 입력한 방향으로 회전하고 카메라는 영향을 받지 않는 형식의 조작을 사용합니다.   
```
ABaseCharacter::ABaseCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
  ...
}
```
  
- 다음과 같은 기본적인 애니메이션 구조 사용, 걷기와 달리기는 BlendSpsace 1D를 통해 Speed 값을 받습니다.   
![animation](https://github.com/sfc7/Action/assets/124154607/652ceb9b-e0d9-4c43-bc8f-82977fe34e92)
  
- 캐릭터 교대는 아래 코드와 같이 부착된 액터들을 제거후 NoCollsion로 바꾸고 바꿀 캐릭터를 스폰한뒤 컨트롤러를 넘겨주고 Destroy()하는 방식을 채택했습니다.   
```
void ABaseCharacter::CharacterChange(TSubclassOf<ABaseCharacter> ChangeCharacterType)
{
	CUI->RemoveFromParent();

	TArray<AActor*> actors;
	GetAttachedActors(actors);
	for (auto actor : actors) {
		actor->Destroy();
	}

	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	auto ChangeCharacter = GetWorld()->SpawnActor<ABaseCharacter>(
		...
	);

	APlayerController* CurrentController = Cast<APlayerController>(GetController());

	if (ChangeCharacter) {
		CurrentController->Possess(ChangeCharacter);
		ChangeCharacter->BaseController = CurrentController;
	}


	Destroy();
}
```

- 특정 Socket에 Weapon을 장착함
   
## 캐릭터 - 카타나
- SweepSingleByChannel을 이용한 근접 공격 사용   
  ![ka](https://github.com/sfc7/Action/assets/124154607/093a8c28-9650-4fbe-98bb-71e22caf7a3e)   
- Q와 R 스킬은 Animation Sequence를 섞어서 만든 Animation Montage들을 스킬들로 사용합니다.   
- Animnotify의 AttackComboCheck 델리게이트를 람다함수로 바인딩 시키고 bool 변수값들을 이용해 콤보 입력을 받아 공격합니다.   
```
AnimInstance->AttackComboCheck.AddLambda([this]() -> void {
		CanNextCombo = false; // 콤보 지속 여부
		if (IsComboInputOn) // 콤보에 대한 선입력 
		{
			AttackStartComboState(); // 지속 여부 = true, 선입력 = false, CurrentCombo = Clamp로 제한
			if (CurrentCombo != MaxCombo) {
				AnimInstance->JumpToAttackMontageSection(CurrentCombo); // Montage 섹션이 넘어감
			}
		}
	});
```
   
## 캐릭터 - 마법
- 모든 공격이 충돌체 판정을 가진 공격 사용
- 기본 공격은 여러 갈래로 Linetrace를 쏘고 hit 되면 ProjectileMovement->HomingAccelerationMagnitude을 높게 준 구체를 유도공격 합니다.   
  ![mp](https://github.com/sfc7/Action/assets/124154607/a0c681dc-c1aa-4915-aed9-16d5b7dc2668)   
```
for(uint8 i = 0; i<60;i++){
EndLocation = StartLocation + UKismetMathLibrary::RotateAngleAxis(GetActorForwardVector(), CurrentAngle, FVector(0.0f, 0.0f, 1.0f)) * 1500.0f;
		bHit = GetWorld()->LineTraceSingleByChannel(
			OUT HitResult,
			StartLocation,
			EndLocation,
			...
		);	
}
   
```  
- Q스킬은 OnComponentOverlap하는 토네이도를 생성하여 공격하는 스킬, 지형과 충돌 시에는 사라집니다.     
  ![mQ](https://github.com/sfc7/Action/assets/124154607/ae4927e4-769d-4ed2-b77a-fac6f631e9dd)   
- R스킬은 하늘에서 OnComponentHit하는 메테오를 떨어뜨려 공격하는 스킬   
  ![mR](https://github.com/sfc7/Action/assets/124154607/7cb70a4a-a77e-4ea4-a466-6a8feceeca65)   
   
## 캐릭터 - 라이플   
- 액션 게임에 총을 사용하는 캐릭터를 위해 다른 캐릭터들과는 추가적인 조작을 구현할 필요가 있었습니다.   
- 다른 캐릭터들과 달리 몇가지 기능을 위해 전투와 비전투모드를 따로 분리할 필요가 있었습니다. ex)조준이 아닌 상태에서 공격등   
   ![idle](https://github.com/sfc7/Action/assets/124154607/109d805d-c844-46e5-b360-3e6a32ed0cdc)   ![idle2](https://github.com/sfc7/Action/assets/124154607/f237860b-61e8-4146-a835-85cda7b7ea4b)
- 총을 조준한 상황에서는 BlendSpace를 1D -> 2D로 교체하고 Aim Offset을 추가한 뒤 아래 코드를 통해 8ways 방식의 이동으로 바뀌도록 하였습니다.   
  
```
GetCharacterMovement()->bOrientRotationToMovement = false;
GetCharacterMovement()->bUseControllerDesiredRotation = true;
```
- AHUD를 상속받는 Crosshair를 만들어 에임을 표시하고 캐릭터의 Speed 값이 바뀌거나 총을 쏠 경우 CrossHair를 벌어지도록 하였습니다.   
![aim1](https://github.com/sfc7/Action/assets/124154607/cdc6975a-774d-48e7-b8ce-2ba5848b428a)
- Q스킬은 레이저 총으로 교체하고 레이저를 한발 발사한 후 교체됩니다.   
![rifle](https://github.com/sfc7/Action/assets/124154607/1dd180d7-9bff-411b-ba50-090068bcc993) ![raser](https://github.com/sfc7/Action/assets/124154607/93a021d4-a253-42e0-92f1-e50dca9442ac)
- R스킬은 PredictProjectilePath_ByObjectType를 통해 포물선 궤적을 계산하고 발사합니다. 도착한 곳에는 돔 형태의 반원이 생성됩니다.
  조작을 통해 반원을 움직여 해당 지점에 포격합니다.   
![GM](https://github.com/sfc7/Action/assets/124154607/7cab9a94-8f6b-481a-9352-9a49b3c54073)  ![GMM](https://github.com/sfc7/Action/assets/124154607/1913f018-ad0e-4088-9dec-67a4a5502b2e)
- 반원은 매태리얼 설정에 의해 범위 안에 있는 액터의 표면을 초록색으로 표시합니다.   
![GR](https://github.com/sfc7/Action/assets/124154607/b0c3a61a-6c7e-46c2-99dd-a1a386273c30)  ![material](https://github.com/sfc7/Action/assets/124154607/07e940bd-0fb1-4734-861f-127163d6f2bf)
- 미사일은 일정 시간 간격으로 일정 거리를 벌리며 발사됩니다.   
그리고 발사된후 타겟이 지정되어야하는데 해당 코드 구현을 위해 TimerHandle 안에서 TimerHandle을 구현하고 매개변수를 call by value로 주었습니다.   
```for (int i = 0; i < 5; i++) {
	FVector CurrentMissileLaunchVector = MissleLaunchVector + GetActorRightVector() * (0.5f * i);
	FTimerHandle LaunchTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(LaunchTimerHandle, FTimerDelegate::CreateLambda([CurrntMissleLaunchVector,this]() {
		auto aMissle = GetWorld()->SpawnActor<AMissle>(AMissle::StaticClass(), GetMesh()->GetSocketTransform("backpack_Socket"), FActorSpawnParameters());
		if (aMissle) {
			Missle->ProjectileMovement->Velocity = CurrntMissleLaunchVector * 1000.0f;
			FTimerHandle MissleHandle;
			GetWorld()->GetTimerManager().SetTimer(MissleHandle, FTimerDelegate::CreateLambda([aMissle, this]() {
				aMissle->SetTarget(TargetLocation);
				}), 0.1f, false);
			MissleTimerHandles.Emplace(MissleHandle);
		}
	}), 0.1f + 0.1f * i, false);
	}
```
   
## 데이터   
### 인스턴스   
- 게임에 필요한 데이터들을 USTRUCT()형태로 선언하여 DataTable로 관리합니다   
- 실시간으로 필요한 캐릭터 교체 쿨타임이나 각 캐릭터들의 스킬 쿨타임등을 인스턴스에서 가져와 사용합니다. 
- 이 때 쿨타임 타이머는 인스턴스에서 SetTimer에 람다함수로 참조 값을 받아 자체적으로 시작합니다.
```
void UMyGameInstance::SkillCoolDownStart(FTimerHandle& SkillHandle, float& CharacterCooldown, float ApplyCooldown, bool& ShouldSkill)
{
	...
	GetWorld()->GetTimerManager().SetTimer(SkillHandle, FTimerDelegate::CreateLambda([this, &SkillHandle, &CharacterCooldown, &ShouldSkill]() {
		CharacterCooldown -= 0.01f;
		if (CharacterCooldown <= 0.0f) {
			GetWorld()->GetTimerManager().ClearTimer(SkillHandle);
			CharacterCooldown = 0.0;
		}
	}), 0.01f, true);
	...
}
```
### 데이터 컴포넌트   
- 컴포넌트는 UActorComponent를 상속받고 인스턴스 등에서 필요한 정보를 가져옵니다.
- 각 캐릭터와 몬스터에서 만들어 사용합니다.   
   
## UI   
- 각 액터들이 가지고 있는 데이터 컴포넌트에서 값을 가져와 연계하여 사용합니다.
- 각 캐릭터의 UI는 생성자에 묶여있고 캐릭터를 교대할 경우에는 UI를 삭제하고 다시 생성하는 방식을 사용합니다.
- 행동들에 대한 쿨타임이 표시됩니다.   
![ui](https://github.com/sfc7/Action/assets/124154607/efa3b80e-8668-4e52-a3a5-4a7db0299946)

## 트리거
- 트리거의 경우 대부분 Box형태를 사용하였습니다. BeginOverlap에 따른 이벤트들을 바인딩 해주었습니다.   
- 장소에 대한 값이 필요할 경우 ArrowComponent를 가진 Pawn을 설치하고 GetAllActorsOfClass로 Level에 해당 Pawn들을 가져와 그 Location에 이벤트를 발생시키는 형태를 주로 사용하였습니다.

## 몬스터   
### AI
- 몬스터 AI는 BeHaviroTree를 통해 구현하였습니다.   
- AI Perception을 이용하여 Stimulus.Type을 통해 Target을 블랙보드에 설정합니다.
- 기본적으로 Sequence 내에서 타겟에 대해 SetFocus, ClearFocus하고 WalkSpeed값을 설정한 뒤 Task를 실행하는 형식을 취하고 있습니다.
![bt](https://github.com/sfc7/Action/assets/124154607/d15044b4-747a-4fd1-afe9-54a0360722d7)

### 공격
- Animation에서 근접 공격과 원거리 공격에 대한 Montage를 각각 Array로 믂고 RandRange로 랜덤하게 패턴이 나오게 합니다.   
- UBTDecorator를 통해 타겟과의 거리를 잰 후 근접공격을 할지 원거리 공격을 할지 결정합니다.

### 보스몬스터   
- EQS를 통해 캐릭터의 주위를 배회합니다.   
- 보스몬스터는 다양한 패턴을 사용합니다.
![Stack](https://github.com/sfc7/Action/assets/124154607/8487a64a-d51b-46a6-858d-04eb5ec90f09)
- 보스몬스터가 사용하는 컨셉 중 가장 중요한 텔레포트의 경우에는 다음과 같은 코드를 통해 구현하였습니다.   
```
auto Movement = GetCharacterMovement();
	Movement->SetMovementMode(EMovementMode::MOVE_Flying);
	Movement->MaxFlySpeed = 5000.0f;
	Movement->MaxAcceleration = 99999.0f;
	GetMesh()->SetVisibility(false, true);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));
```   
- 보스몬스터가 사용하는 패턴 중 자기 힐을 하는 패턴은 EQS를 통해 플레이어의 시야 중 보이지 않는 곳을 찾아가는 방식으로 구현했습니다.   
![heal](https://github.com/sfc7/Action/assets/124154607/f90d5a20-815a-492c-9741-3cea823dc596)
 
