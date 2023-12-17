// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"

#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SideScroller/Characters/BasePaperCharacter.h"
#include "SideScroller/Interfaces/ProjectileInterface.h"

/**
 * Initializes the ABaseProjectile instance.
 *
 * @param ProjectileFlipbook The flipbook component used to display the projectile.
 * @param ProjectileBox The collision component used for detecting collisions.
 * @param ProjectileMovementComp The movement component used for controlling the projectile's movement.
 *
 * @return None.
 */
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
	SetRootComponent(ProjectileFlipbook);
	
	this->bReplicates = true;
    ProjectileFlipbook->SetIsReplicated(true);
    ProjectileBox->SetIsReplicated(true);
    ProjectileMovementComp->SetIsReplicated(true);
}

/**
 * Begins the play of the projectile.
 *
 * This method initializes the projectile's collision settings and sets the initial and maximum movement speed.
 */
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	this->ProjectileBox->SetNotifyRigidBodyCollision(true);
	this->ProjectileBox->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);

	ProjectileMovementComp->InitialSpeed = MovementSpeed;
	ProjectileMovementComp->MaxSpeed = MovementSpeed;
}

/**
 * @brief Gets the movement speed of the projectile.
 *
 * @return The movement speed of the projectile.
 */
float ABaseProjectile::GetMovementSpeed() const
{
	return MovementSpeed;
}

/**
 * @brief Get the Projectile Movement Component
 *
 * This method returns the Projectile Movement Component associated with the current projectile object.
 *
 * @return A pointer to the UProjectileMovementComponent, or nullptr if none is found.
 */
UProjectileMovementComponent* ABaseProjectile::GetProjectileMovementComp() const
{
	return ProjectileMovementComp;
}

/**
 * @brief Get the flipbook component of the projectile
 *
 * @return UPaperFlipbookComponent* - The flipbook component of the projectile
 */
UPaperFlipbookComponent* ABaseProjectile::GetProjectileFlipbook() const
{
	return ProjectileFlipbook;
}

/**
 * Launches the projectile in the specified direction.
 *
 * @param Direction The direction in which to launch the projectile.
 *
 * @note The projectile must have an owner associated with it in order for it to be launched. If the projectile does
 * not have an owner, a warning message will be logged and the function will exit without performing
 * any further actions.
 *
 * If the owner of the projectile implements the UProjectileInterface, the SetProjectileTransform() method will
 * be called on the owner to adjust the projectile's transform based on the direction, owner, and base
 * character information.
 *
 * The lifespan of the projectile will be set to the value specified by the ProjectileInLifespan property.
 *
 * The LaunchSound will be played and attached to the ProjectileFlipbook, with the tag "ProjectileLaunch".
 *
 * A verbose log message will be outputted indicating the name of the projectile and its owner.
 */
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

/**
 * The OnHit method is called when the projectile hits another component or actor.
 *
 * @param HitComp The primitive component that was hit.
 * @param OtherActor The actor that was hit.
 * @param OtherComp The primitive component that was hit on the other actor.
 * @param NormalImpulse The impulse applied to the hit component.
 * @param Hit The hit result containing detailed information about the hit.
 */
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

/**
 * \brief Performs collision animation and sound effects for the projectile.
 *
 * This method is responsible for playing the collision animation, stopping the projectile movement,
 * disabling collision, setting the appropriate loop and flipbook for the projectile,
 * setting a timer to destroy the projectile actor after the collision animation has finished,
 * playing a hit sound at the collision location, and inflicting damage to the other character actor.
 *
 * \param MyOwner The owner of the projectile.
 * \param OtherBasePaperActor The other character actor that the projectile has collided with.
 */
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

/**
 * Destroys the actor and clears the collision timer.
 *
 * @param None
 * @return None
 */
void ABaseProjectile::DestroyActor()
{
	UE_LOG(LogTemp, Display, TEXT("Destroying %s!"), *this->GetName());
	this->Destroy();
	GetWorld()->GetTimerManager().ClearTimer(this->CollisionTimerHandle);
}

