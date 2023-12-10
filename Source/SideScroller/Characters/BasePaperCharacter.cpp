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

void ABasePaperCharacter::DoDeath()
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

	UGameplayStatics::SpawnSoundAttached(
		this->PainSound,
		this->GetSprite(),
		TEXT("BasePaperCharacterPain")
	);
}

void ABasePaperCharacter::HurtFinishedCallback()
{
	this->GetSprite()->SetFlipbook(IdleAnimation);
}

bool ABasePaperCharacter::IsDead() const
{
	return this->bIsDead;
}

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
				PlayerFoxVictim->PlayerDeath();
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
