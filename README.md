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
### 공통 - 기본
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
