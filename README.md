# Action
- 사용엔진 Unreal 5.2
  
- 장르 : 3인칭 액션

영상:
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
- 캐릭터는 입력한 방향으로 회전하고 카메라는 영향을 받지 않는 형식의 조작 지원
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
  
- 다음과 같은 기본적인 애니메이션 구조 사용, 걷기와 달리기는 BS 1D를 통해 Speed 값을 받음 
![animation](https://github.com/sfc7/Action/assets/124154607/652ceb9b-e0d9-4c43-bc8f-82977fe34e92)
  
- 캐릭터 교대는 아래 코드와 같이 부착된 액터들을 제거후 NoCollsion로 바꾸고 바꿀 캐릭터를 스폰한뒤 컨트롤러를 넘겨주고 Destroy()하는 방식을 채택 
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
- Q와 R 스킬은 Animation Sequence를 섞어서 만든 Animation Montage들을 스킬들로 사용함
- Animnotify의 AttackComboCheck 델리게이트를 람다함수로 바인딩 시키고 bool 변수값들을 이용해 콤보 입력을 받아 공격함
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
- 기본 공격은 여러 갈래로 Linetrace를 쏘고 hit 되면 ProjectileMovement->HomingAccelerationMagnitude을 높게 준 구체를 유도공격 함   
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
- Q스킬은 OnComponentOverlap하는 토네이도를 생성하여 공격하는 스킬   
  ![mQ](https://github.com/sfc7/Action/assets/124154607/ae4927e4-769d-4ed2-b77a-fac6f631e9dd)   
- R스킬은 하늘에서 OnComponentHit하는 메테오를 떨어뜨려 공격하는 스킬   
  ![mR](https://github.com/sfc7/Action/assets/124154607/7cb70a4a-a77e-4ea4-a466-6a8feceeca65)   

