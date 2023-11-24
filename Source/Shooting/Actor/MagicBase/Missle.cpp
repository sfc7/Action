#include "Missle.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMissle::AMissle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Speed = 1100.f;
	Gravity = 0.0f;

	CollisionMesh = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionMesh"));
	SetRootComponent(CollisionMesh);
	CollisionMesh->SetRelativeScale3D(FVector(4.0f, 1.0f, 1.0f));
	CollisionMesh->SetCollisionProfileName(TEXT("MagicBall"));

	StaticMesh = CreateDefaultSubobject	<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(CollisionMesh);
	StaticMesh->SetRelativeRotation(FRotator(90.0f, 0.0f, 90.0f));
	StaticMesh->SetRelativeScale3D(FVector(3.0f, 3.0f, 6.0f));
	StaticMesh->SetRelativeLocation(FVector(-30.0f, 0.0f, 0.0f));
	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
	
	static ConstructorHelpers::FObjectFinder <UStaticMesh> MissleAsset(TEXT("/Script/Engine.StaticMesh'/Game/Shooting/BluePrint/Effect/Magic/Mesh/SM_Shinbi_Sword_Surround.SM_Shinbi_Sword_Surround'"));
	if (MissleAsset.Succeeded()) {
		StaticMesh->SetStaticMesh(MissleAsset.Object);
	}

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystem->SetupAttachment(StaticMesh);
	ParticleSystem->SetRelativeScale3D(FVector(0.3f, 0.3f, 0.3f));
	ParticleSystem->SetRelativeLocation(FVector(-1.0f, 50.0f, -5.0f));

	ConstructorHelpers::FObjectFinder<UParticleSystem> SmokeEffectAsset(TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeEffects/Effects/FX_Ambient/Fire/P_Fire_wallTorch_Blue_noSmoke.P_Fire_wallTorch_Blue_noSmoke'"));
	if (SmokeEffectAsset.Succeeded()) {
		SmokeEffect = SmokeEffectAsset.Object;
		ParticleSystem->SetTemplate(SmokeEffect);
		ParticleSystem->CustomTimeDilation = 5.0f;
	}

	ConstructorHelpers::FObjectFinder<UParticleSystem> ImpactEffectAsset(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonShinbi/FX/Particles/Abilities/Ultimate/FX/P_Mudang_Ult_Wolf_End.P_Mudang_Ult_Wolf_End'"));
	if (ImpactEffectAsset.Succeeded()) {
		ImpactEffect = ImpactEffectAsset.Object;
	}

	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForce->SetupAttachment(ParticleSystem);
	RadialForce->Radius = 300.0f;
	RadialForce->bImpulseVelChange = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(RootComponent);

	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bInitialVelocityInLocalSpace = true;
	ProjectileMovement->ProjectileGravityScale = Gravity;
	ProjectileMovement->HomingAccelerationMagnitude = 5000.0f;

	HomingTarget = CreateDefaultSubobject<USceneComponent>(TEXT("HomingTarget"));
}

// Called when the game starts or when spawned
void AMissle::BeginPlay()
{
	Super::BeginPlay();

	CollisionMesh->OnComponentHit.AddDynamic(this, &AMissle::ProjectileImpact);
}

// Called every frame
void AMissle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMissle::ProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComponent != nullptr) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, Hit.ImpactPoint.Rotation(), FVector(3.0f, 3.0f, 3.0f));
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		TArray<AActor*> IgnoreActors;
		TArray<AActor*> OutActors;

		UKismetSystemLibrary::SphereOverlapActors(
			this,
			RadialForce->GetComponentLocation(),
			RadialForce->Radius,
			ObjectTypes,
			AActor::StaticClass(),
			IgnoreActors,
			OutActors
		);

		for (AActor* Actor : OutActors)
		{
			if (Actor && Actor->GetClass()->GetSuperClass()->GetName() == TEXT("BaseMonster")) {
				UGameplayStatics::ApplyDamage(Actor, 10.0f, GetInstigatorController(), this, NULL);
			}
		}
		Destroy();
	}
}
void AMissle::SetTarget(FVector& TargetLocation)
{
	if (IsValid(HomingTarget)) {
		HomingTarget->SetWorldLocation(TargetLocation);
		ProjectileMovement->bIsHomingProjectile = true;
		ProjectileMovement->HomingTargetComponent = HomingTarget;
	}
}


