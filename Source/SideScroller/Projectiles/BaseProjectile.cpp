// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"

#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SideScroller/Characters/BasePaperCharacter.h"
#include "SideScroller/Interfaces/ProjectileInterface.h"

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
	
	this->bReplicates = true;
    ProjectileFlipbook->SetIsReplicated(true);
    ProjectileBox->SetIsReplicated(true);
    ProjectileMovementComp->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	this->ProjectileBox->SetNotifyRigidBodyCollision(true);
	this->ProjectileBox->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);

	ProjectileMovementComp->InitialSpeed = MovementSpeed;
	ProjectileMovementComp->MaxSpeed = MovementSpeed;
}

float ABaseProjectile::GetMovementSpeed() const
{
	return MovementSpeed;
}

UProjectileMovementComponent* ABaseProjectile::GetProjectileMovementComp() const
{
	return ProjectileMovementComp;
}

UPaperFlipbookComponent* ABaseProjectile::GetProjectileFlipbook() const
{
	return ProjectileFlipbook;
}

void ABaseProjectile::LaunchProjectile(const float Direction)
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("ABaseProjectile::LaunchProjectile - %s has no owner! Exiting LaunchProjectile Function."),
			*this->GetName()
		);
		return;
	}
	UE_LOG(LogTemp, Verbose, TEXT("ABaseProjectile::LaunchProjectile - %s's  owner is %s."),
		*this->GetName(),
		*GetOwner()->GetName()
	);

	const ABasePaperCharacter* BaseChar = dynamic_cast<ABasePaperCharacter*>(MyOwner);
	if (BaseChar == nullptr) return;

	UGameplayStatics::SpawnSoundAttached(
		this->LaunchSound,
		this->ProjectileFlipbook,
		TEXT("ProjectileLaunch")
	);
	
	this->SetLifeSpan(ProjectileInLifespan);

	if (MyOwner->GetClass()->ImplementsInterface(UProjectileInterface::StaticClass()))
	{
		Cast<IProjectileInterface>(MyOwner)->SetProjectileTransform(Direction, MyOwner, BaseChar, this);
	}

	UE_LOG(LogTemp, Verbose,
	       TEXT("ABaseProjectile::LaunchProjectile - %s has rotation %s."),
	       *this->ProjectileFlipbook->GetName(),
	       *this->ProjectileFlipbook->GetRelativeRotation().ToString()
	);
}

void ABaseProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                            FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Verbose, TEXT("hit something."));

	const AActor* MyOwner = GetOwner();
	if (!MyOwner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Component has no owner! Exiting OnHit Function."));
		return;
	}
	UE_LOG(
		LogTemp,
		Display,
		TEXT("ABaseProjectile::OnHit - The owner of projectile, %s, is %s."),
		*this->GetName(),
		*MyOwner->GetName()
	);

	// this keeps enemies from shooting each other
	if (MyOwner->GetName().Contains("Enemy") && OtherActor->GetName().Contains("Enemy"))
	{
		return;
	}

	ABasePaperCharacter* OtherBasePaperActor = dynamic_cast<ABasePaperCharacter*>(OtherActor);
	if (OtherBasePaperActor == nullptr) return;
	
	UE_LOG(LogTemp, Display,
		TEXT("ABaseProjectile::OnHit - %s has hit %s."),
		*this->GetName(), *OtherBasePaperActor->GetName()
	);
	
	if (OtherBasePaperActor != MyOwner)
	{
		DoCollisionAnimAndSound(MyOwner, OtherBasePaperActor);
	}
}

void ABaseProjectile::DoCollisionAnimAndSound(const AActor* MyOwner, ABasePaperCharacter* OtherBasePaperActor)
{
	this->SetLifeSpan(CollisionAnimationTime);
	this->ProjectileMovementComp->StopMovementImmediately();
	this->SetActorEnableCollision(false);
	this->GetProjectileFlipbook()->SetLooping(false);
	this->GetProjectileFlipbook()->SetFlipbook(CollisionAnimation);
	GetWorld()->GetTimerManager().SetTimer(
		this->CollisionTimerHandle,
		this,
		&ABaseProjectile::DestroyActor,
		CollisionAnimationTime,
		false
	);
		
	UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	OtherBasePaperActor->TakeDamage(
		Damage,
		FDamageEvent(UDamageType::StaticClass()),
		MyOwner->GetInstigatorController(),
		this
	);
}

void ABaseProjectile::DestroyActor()
{
	UE_LOG(LogTemp, Display, TEXT("Destroying %s!"), *this->GetName());
	this->Destroy();
	GetWorld()->GetTimerManager().ClearTimer(this->CollisionTimerHandle);
}

