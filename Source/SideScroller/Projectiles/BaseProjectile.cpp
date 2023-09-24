// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"

#include "PaperFlipbookComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SideScroller/Characters/BasePaperCharacter.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("ProjectileFlipbook"));
	ProjectileFlipbook->SetupAttachment(RootComponent);

	ProjectileBox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("ProjectileCollision"));
	ProjectileBox->SetupAttachment(ProjectileFlipbook);
	ProjectileBox->SetHiddenInGame(true);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	this->ProjectileBox->SetNotifyRigidBodyCollision(true);
	this->ProjectileBox->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);

	ProjectileMovementComp->ProjectileGravityScale = 0.f;
	ProjectileMovementComp->InitialSpeed = MovementSpeed;
	ProjectileMovementComp->MaxSpeed = MovementSpeed;
}

void ABaseProjectile::LaunchProjectile(const float XDirection)
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABaseProjectile::LaunchProjectile - Component has no owner! Exiting OnHit Function."));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("ABaseProjectile::LaunchProjectile - Projectile's  owner is %s."), *GetOwner()->GetName());

	const ABasePaperCharacter* BaseChar = dynamic_cast<ABasePaperCharacter*>(MyOwner);
	if (BaseChar == nullptr) return;

	this->SetLifeSpan(0.5f);
	ProjectileMovementComp->Velocity = FVector(XDirection * MovementSpeed, 0 , 0);
}

void ABaseProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                            FVector NormalImpulse, const FHitResult& Hit)
{
	const AActor* MyOwner = GetOwner();
	if (!MyOwner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Component has no owner! Exiting OnHit Function."));
		return;
	}
	UE_LOG(
		LogTemp,
		Warning,
		TEXT("ABaseProjectile::OnHit - The owner of projectile, %s, is %s."),
		*this->GetName(),
		*MyOwner->GetName()
	);

	ABasePaperCharacter* OtherBasePaperActor = dynamic_cast<ABasePaperCharacter*>(OtherActor);
	if (OtherBasePaperActor == nullptr) return;

	UE_LOG(LogTemp, Warning,
		TEXT("ABaseProjectile::OnHit - %s has hit %s."),
		*this->GetName(), *OtherBasePaperActor->GetName()
	);
	
	if (OtherBasePaperActor != MyOwner)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		OtherBasePaperActor->TakeDamage(
			Damage,
			FDamageEvent(UDamageType::StaticClass()),
			MyOwner->GetInstigatorController(),
			this
		);
		Destroy();
	}
}

