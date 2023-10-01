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

	ProjectileMovementComp->InitialSpeed = MovementSpeed;
	ProjectileMovementComp->MaxSpeed = MovementSpeed;
}

UProjectileMovementComponent* ABaseProjectile::GetProjectileMovementComp() const
{
	return ProjectileMovementComp;
}

UPaperFlipbookComponent* ABaseProjectile::GetProjectileFlipbook() const
{
	return ProjectileFlipbook;
}

void ABaseProjectile::GetEnemyPlayerPitch(
	const ABasePaperCharacter* BaseChar,
	FRotator& OwnerRotation,
	float Direction
) const {
	OwnerRotation = BaseChar->GetArrowComponent()->GetComponentRotation();
	UE_LOG(LogTemp, Warning,
		TEXT("ABaseProjectile::GetEnemyPlayerPitch - %s's (owner) rotation is %s."),
		*BaseChar->GetName(), *OwnerRotation.ToString()
	);

	const FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetTargetLocation();
	UE_LOG(LogTemp, Warning,
		TEXT("ABaseProjectile::GetEnemyPlayerPitch - %s's (player) location is %s."),
		*GetWorld()->GetFirstPlayerController()->GetName(), *PlayerLocation.ToString()
	);
	
	const FVector OwnerLocation = BaseChar->GetActorLocation();
	UE_LOG(LogTemp, Warning,
		TEXT("ABaseProjectile::GetEnemyPlayerPitch - %s's (owner) location is %s."),
		*BaseChar->GetName(), *OwnerLocation.ToString()
	);
	
	const FVector ProjectileVector = PlayerLocation - OwnerLocation;
	UE_LOG(LogTemp, Warning,
		TEXT("ABaseProjectile::GetEnemyPlayerPitch - projectile vector should be %s."),
		*ProjectileVector.ToString()
	);

	const float DzDx = ProjectileVector.Z / ProjectileVector.X;
	UE_LOG(LogTemp, Warning, TEXT("ABaseProjectile::GetEnemyPlayerPitch - DzDx should be %f."), DzDx);

	const float PitchRadians = atan(DzDx);
	UE_LOG(LogTemp, Warning, TEXT("ABaseProjectile::GetEnemyPlayerPitch - PitchRadians should be %f."), PitchRadians);
	
	const float ProjectilePitch = Direction * (180 * PitchRadians / PI);
	UE_LOG(LogTemp, Warning, TEXT("ABaseProjectile::GetEnemyPlayerPitch - PitchDegrees should be %f."), ProjectilePitch);
	
	OwnerRotation = FRotator(ProjectilePitch, OwnerRotation.Yaw, OwnerRotation.Roll);
	UE_LOG(LogTemp, Warning,
		TEXT("ABaseProjectile::GetEnemyPlayerPitch - Rotation from owner to player is %s."),
		*OwnerRotation.ToString()
	);
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
	UE_LOG(LogTemp, Warning, TEXT("ABaseProjectile::LaunchProjectile - %s's  owner is %s."),
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

	if (BaseChar->GetShootUpward())
	{
		this->ProjectileFlipbook->SetRelativeRotation(FRotator(90, 0, 0));
		ProjectileMovementComp->Velocity = FVector(0.f, 0.f, Direction * MovementSpeed);
	}
	else
	{
		FRotator OwnerRotation;
		if (MyOwner->GetName().Contains("Player"))
		{
			OwnerRotation = BaseChar->GetSprite()->GetRelativeRotation();
		}
		else
		{
			if (MyOwner->GetName().Contains("Eagle"))
			{
				GetEnemyPlayerPitch(BaseChar, OwnerRotation, Direction);
			} else
			{
				OwnerRotation = BaseChar->GetArrowComponent()->GetComponentRotation();
			}
		}
		
		this->ProjectileFlipbook->SetRelativeRotation(OwnerRotation);
		ProjectileMovementComp->Velocity = FVector(Direction * MovementSpeed, 0.f, 0.f);
	}

	UE_LOG(LogTemp, Warning,
	       TEXT("ABaseProjectile::LaunchProjectile - %s has rotation %s."),
	       *this->ProjectileFlipbook->GetName(),
	       *this->ProjectileFlipbook->GetRelativeRotation().ToString()
	);
}

void ABaseProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                            FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("hit something."));

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
	UE_LOG(LogTemp, Warning, TEXT("Destroying %s!"), *this->GetName());
	this->Destroy();
	GetWorld()->GetTimerManager().ClearTimer(this->CollisionTimerHandle);
}

