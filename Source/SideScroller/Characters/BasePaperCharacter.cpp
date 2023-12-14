// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePaperCharacter.h"

#include "Enemies/EnemyCollisionPaperCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

#include "Players/PC_PlayerFox.h"

ABasePaperCharacter::ABasePaperCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	Health = DefaultHealth;

	this->GetCharacterMovement()->bConstrainToPlane = true;
	this->GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0, 1, 0));

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);

	this->ProjectileSpawnPoint->SetIsReplicated(true);
	this->GetCharacterMovement()->SetIsReplicated(true);
	this->GetSprite()->SetIsReplicated(true);
	this->SetReplicates(true);
}

void ABasePaperCharacter::BeginPlay()
{
	Super::BeginPlay();
	ProjectileSpawnPoint->SetRelativeLocation(ProjectileSpawnLoc);
}

/**
 * Retrieves the properties that should be replicated for this character's lifetime.
 *
 * @param OutLifetimeProps - The array that will contain the replicated properties.
 */
void ABasePaperCharacter::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABasePaperCharacter, Health);
}

float ABasePaperCharacter::GetDamage() const
{
	return Damage;
}

void ABasePaperCharacter::SetDamage(const float DamageAmount)
{
	this->Damage = DamageAmount;
}

/**
 * Adds the specified value to the character's health.
 *
 * This function increases the character's current health by the specified value, clamping the result between 0
 * and the character's default health.
 *
 * @param HealthValue The value to add to the character's health.
 */
void ABasePaperCharacter::AddHealth(const float HealthValue)
{
	this->SetHealth(FMath::Clamp(this->Health + HealthValue, 0.f, this->DefaultHealth));
}

void ABasePaperCharacter::SetHealth(const float HealthValue)
{
	this->Health = HealthValue;
}

float ABasePaperCharacter::GetDefaultHealth() const
{
	return DefaultHealth;
}

float ABasePaperCharacter::GetHealth() const
{
	return Health;
}

USceneComponent* ABasePaperCharacter::GetProjectileSpawnPoint() const
{
	return ProjectileSpawnPoint;
}

/**
 * Implements the logic for character death.
 *
 * Marks the character as dead, disables collision, sets death animation, and spawns death sound.
 * Finally, sets a timer to destroy the actor after the death animation finishes playing.
 */
void ABasePaperCharacter::DoDeath_Implementation()
{
	this->bIsDead = true;
	UE_LOG(LogTemp, Display, TEXT("%s's health depleted!"), *this->GetName());
	this->SetActorEnableCollision(false);
	this->GetSprite()->SetLooping(false);
	this->GetSprite()->SetFlipbook(DeathAnimation);
	GetWorld()->GetTimerManager().SetTimer(
		this->DeathTimerHandle,
		this,
		&ABasePaperCharacter::DestroyActor,
		DeathAnimationTime,
		false
	);

	UGameplayStatics::SpawnSoundAttached(
		this->DeathSound,
		this->GetSprite(),
		TEXT("BasePaperCharacterDeath")
	);
}

/**
 * Moves the character some distance away from the enemy after damage has been incurred.
 *
 * @param DamageCauser The actor causing the damage.
 */
void ABasePaperCharacter::PushHurtCharacter(AActor* DamageCauser)
{
	// move character some distance away from the enemy after damage incurred
	const FVector DamageCauserLocation = DamageCauser->GetActorLocation();
	const FVector PlayerLocation = this->GetActorLocation();

	float HurtPush = UECasts_Private::DynamicCast<APC_PlayerFox*>(this)->HurtPushAmount;
	if (DamageCauserLocation.X - PlayerLocation.X > 0)
	{
		HurtPush *= -1.f;
	}
	// for player knock-back, use launch to be sent away from enemy
	this->LaunchCharacter(FVector(HurtPush, 0.0, 0.0), false, false);
}

/**
 * @param None
 *
 * PlayHurtSound_Implementation is a method implemented in the ABasePaperCharacter class that is used to play a pain
 * sound when the character is hurt.
 
 * The method uses the UGameplayStatics::SpawnSoundAttached function to spawn a sound attached to the character's sprite.
 * The pain sound to be played is specified by the PainSound member
 * variable of the character.
 *
 * @return None
 */
void ABasePaperCharacter::PlayHurtSound_Implementation()
{
	UGameplayStatics::SpawnSoundAttached(
		this->PainSound,
		this->GetSprite(),
		TEXT("BasePaperCharacterPain")
	);
}

/**
 * @brief Inflict damage to the character and trigger the hurt sequence.
 *
 * This method is called to inflict damage to the character and trigger the hurt sequence. It is primarily designed
 * for use in games and is specific to the ABasePaperCharacter class. It changes the character's sprite flipbook to
 * display the hurt animation, sets a timer to restore the original sprite after a specified time, and calls the
 * PushHurtCharacter method to apply various effects based on the damage causer. Finally, it plays a hurt sound effect.
 *
 * @param DamageCauser A pointer to the actor that caused the damage.
 */
void ABasePaperCharacter::DoHurt(AActor* DamageCauser)
{
	if (const APC_PlayerFox* PlayerFox = dynamic_cast<APC_PlayerFox*>(this);
		PlayerFox != nullptr
	) {
		this->GetSprite()->SetFlipbook(HurtAnimation);
		
		GetWorld()->GetTimerManager().SetTimer(
			this->HurtTimerHandle,
			this,
			&ABasePaperCharacter::HurtFinishedCallback,
			HurtAnimationTime,
			false
		);
		
		PushHurtCharacter(DamageCauser);
	}

	PlayHurtSound();
}

void ABasePaperCharacter::HurtFinishedCallback()
{
	this->GetSprite()->SetFlipbook(IdleAnimation);
}

bool ABasePaperCharacter::IsDead() const
{
	return this->bIsDead;
}

/**
 * Calculate the angle between the character's up vector and the floor normal.
 *
 * @return The angle between the character's up vector and the floor normal in degrees.
 */
float ABasePaperCharacter::GetFloorAngle()
{
	const FFindFloorResult TheFloor = this->GetCharacterMovement()->CurrentFloor;
	const FVector FloorNormal = TheFloor.HitResult.Normal;
	const FVector CharUpVector = this->GetActorUpVector();
	const float DeltaX = CharUpVector.X - FloorNormal.X;
	return asin(DeltaX) * 180.f / PI;
}

float ABasePaperCharacter::GetShootDelayTime() const
{
	return ShootDelayTime;
}

/**
 * Method to try giving points and then perform death.
 *
 * This method is used to give points to the attacking player and perform death if the subject of damage is not a Player.
 * It checks if the subject is a Player character, and if not, it retrieves the controller, pawn, and pawn class.
 * If the pawn class implements the PointsInterface, it calls the GivePoints() method on itself and then performs death.
 *
 * @param DamageCauser The PlayerFox character that caused the damage.
 */
void ABasePaperCharacter::TryGivingPointsThenDoDeath(APC_PlayerFox* DamageCauser)
{
	// if the subject (this) of damage is not a Player give points
	if (const APC_PlayerFox* PlayerFox = UECasts_Private::DynamicCast<APC_PlayerFox*>(this);
		PlayerFox == nullptr
	) {
		const AController* ThisController = this->GetController();
		if (ThisController == nullptr)
		{
			UE_LOG(LogTemp, Warning,
				TEXT("ABasePaperCharacter::TryGivingPointsThenDoDeath - ThisController is null, no points or death.")
			)
			return;
		}

		const APawn* ThisPawn = ThisController->GetPawn();
		if (ThisPawn == nullptr)
		{
			UE_LOG(LogTemp, Warning,
				TEXT("ABasePaperCharacter::TryGivingPointsThenDoDeath - ThisPawn is null, no points or death.")
			)
			return;
		}

		const UClass* ThisPawnClass = ThisPawn->GetClass();
		if (ThisPawnClass == nullptr)
		{
			UE_LOG(LogTemp, Warning,
				TEXT("ABasePaperCharacter::TryGivingPointsThenDoDeath - ThisPawnClass is null, no points or death.")
			)
			return;
		}
		
		if (ThisPawnClass->ImplementsInterface(UPointsInterface::StaticClass()))
		{
			Cast<IPointsInterface>(this)->GivePoints(DamageCauser);
			DoDeath();
		}
	}
}

float ABasePaperCharacter::TakeDamage(
	float DamageAmount,
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser
)
{
	TakeDamageRPC(DamageAmount, DamageCauser);
	return DamageAmount;
}

/**
 * Takes damage from a damage causer and updates the health of the character.
 *
 * @param DamageAmount The amount of damage to be applied.
 * @param DamageCauser The actor that caused the damage.
 */
void ABasePaperCharacter::TakeDamageRPC_Implementation(float DamageAmount, AActor* DamageCauser)
{
	this->AddHealth(-DamageAmount);
	UE_LOG(LogTemp, Verbose, TEXT("%s's health: %f"), *this->GetName(), this->GetHealth());
	
	if (this->GetHealth() <= 0)
	{
		if (APC_PlayerFox* PlayerFoxDamageCauser = dynamic_cast<APC_PlayerFox*>(DamageCauser)) {
			// the damage causer is the player, so give the player points if the object implements points interface
			TryGivingPointsThenDoDeath(PlayerFoxDamageCauser);
		} else if (APC_PlayerFox* ParentOfDamageCauser = dynamic_cast<APC_PlayerFox*>(DamageCauser->GetOwner())) {
			// if a PlayerFox's projectile was the damage causer
			TryGivingPointsThenDoDeath(ParentOfDamageCauser);
		} else {  // not a player doing the damage
			// if player is the victim
			if (APC_PlayerFox* PlayerFoxVictim = dynamic_cast<APC_PlayerFox*>(this)) {
				PlayerFoxVictim->HandlePlayerDeath();
			}
		}
	} else {
		DoHurt(DamageCauser);
	}
}

bool ABasePaperCharacter::TakeDamageRPC_Validate(float DamageAmount, AActor* DamageCauser)
{
	return true;
}

/**
 * Prepares the launch of a projectile.
 *
 * This method is responsible for spawning a projectile and preparing it for launch.
 *
 * @param bIsPlayer Determines if the caller is a player.
 *                  If true, the sprite's relative rotation is used to calculate the launch direction.
 *                  If false, the actor's rotation is used to calculate the launch direction.
 */
void ABasePaperCharacter::PrepProjectileLaunch(bool bIsPLayer = true)
{
	float Yaw;
	if (bIsPLayer)
	{
		Yaw = abs(round(this->GetSprite()->GetRelativeRotation().Yaw));
	}
	else
	{
		if (this->GetSprite() == nullptr) return;
		Yaw = abs(round(this->GetActorRotation().Yaw));
	}
	
	float Direction = 1.f;
	// if the sprite is close to 180 degrees, set direction to negative
	if (Yaw == 180.0)
	{
		Direction = -1.f;
	}
	
	if (ProjectileClass)
	{
		const FVector ProjectileSpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		const FRotator ProjectileSpawnRotation = ProjectileSpawnPoint->GetComponentRotation();
		ABaseProjectile* TempProjectile = GetWorld()->SpawnActor<ABaseProjectile>(
			ProjectileClass,
			ProjectileSpawnLocation,
			ProjectileSpawnRotation
		);
		
		TempProjectile->SetOwner(this);
		TempProjectile->LaunchProjectile(Direction);
		UE_LOG(
			LogTemp, Verbose, TEXT("ABasePaperCharacter::PrepProjectileLaunch - Owner of spawned projectile, %s, is %s!"),
			*TempProjectile->GetName(),
			*TempProjectile->GetOwner()->GetName()
		);
	}
	else
	{
		UE_LOG(
			LogTemp, Warning, TEXT("ABasePaperCharacter::PrepProjectileLaunch - %s has no projectile class set!"),
			*this->GetName()
		);
	}
}

/**
 * @brief Checks if the player can shoot a projectile.
 *
 * @return Returns true if the player can shoot; false otherwise.
 *
 * @param None.
 */
bool ABasePaperCharacter::PlayerCanShoot()
{
	APC_PlayerFox* Player = UECasts_Private::DynamicCast<APC_PlayerFox*>(this);
	if (Player != nullptr)
	{
		if (Player->GetMovementComponent()->IsFalling()) return false;

		int NumCherries = Player->GetCherryCount();
		if (NumCherries > 0)
		{
			NumCherries -= 1;
			UE_LOG(LogTemp, VeryVerbose, TEXT("ABasePaperCharacter::PlayerCanShoot - Subtract 1 cherry; now, %s has %d cherries"),
			       *Player->GetName(),
			       NumCherries
			);
			Player->SetCherryStash(NumCherries);
			return true;
		}
		
		UE_LOG(LogTemp, Display, TEXT("ABasePaperCharacter::PlayerCanShoot - Can't shoot; %s has %d cherries"),
		       *Player->GetName(),
		       NumCherries
		)
	}
	return false;
}

/**
 * Checks if the enemy character can shoot.
 *
 * @return true if the enemy character is able to shoot, false otherwise.
 *
 * @param none
 *
 */
bool ABasePaperCharacter::EnemyCanShoot()
{
	AEnemyCollisionPaperCharacter* EnemyAI = UECasts_Private::DynamicCast<AEnemyCollisionPaperCharacter*>(this);
	if (EnemyAI)
	{
		if (EnemyAI->bIsDead)
		{
			UE_LOG(LogTemp, Verbose, TEXT("ABasePaperCharacter::EnemyCanShoot - %s is deadand cannot shoot."), *EnemyAI->GetName());
			return false;
		}
		
		UE_LOG(LogTemp, Verbose, TEXT("ABasePaperCharacter::EnemyCanShoot - %s is shooting."), *EnemyAI->GetName());
		return true;
	}

	UE_LOG(
		LogTemp, Warning, TEXT("ABasePaperCharacter::EnemyCanShoot - Cannot cast %s to AEnemyCollisionPaperCharacter."),
		*this->GetName()
	);
	return false;
}

/**
 * @brief Shoots a projectile if the player or enemy can shoot.
 *
 * This method checks if the player or enemy can shoot by calling the PlayerCanShoot() and EnemyCanShoot() methods.
 * If the player can shoot, it prepares and launches the projectile by calling the PrepProjectileLaunch(true) method.
 * If the enemy can shoot, it prepares and launches the projectile by calling the PrepProjectileLaunch(false) method.
 */
void ABasePaperCharacter::Shoot()
{
	if (PlayerCanShoot())
	{
		PrepProjectileLaunch(true);
	}
	else if (EnemyCanShoot())
	{
		PrepProjectileLaunch(false);
	}
}

/**
 * @brief Destroys the actor.
 *
 * This method is used to destroy the actor. If the actor is of type "PlayFox", it will not be destroyed and instead,
 * the "DeathCleanUp()" method of "PlayFox" will be called.
 *
 * @param None.
 *
 * @return None.
 */
void ABasePaperCharacter::DestroyActor()
{
	// early return if this is a PlayFox because we dont want to destroy players so they can spectate
	if (APC_PlayerFox* PlayerFox = UECasts_Private::DynamicCast<APC_PlayerFox*>(this);
		PlayerFox != nullptr
	) {
		UE_LOG(LogTemp, Display, TEXT("Cleaning up, not destroying, %s!"), *this->GetName());
		PlayerFox->DeathCleanUp();
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("Destroying %s!"), *this->GetName());
	this->Destroy();
	GetWorld()->GetTimerManager().ClearTimer(this->DeathTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(this->HurtTimerHandle);
}
