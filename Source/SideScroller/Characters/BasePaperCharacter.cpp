// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePaperCharacter.h"

#include "Enemies/EnemyCollisionPaperCharacter.h"
#include "PaperFlipbookComponent.h"
#include "PC_AIController.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Players/PC_PlayerFox.h"
#include "Tasks/Task.h"

ABasePaperCharacter::ABasePaperCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	Health = DefaultHealth;

	this->GetCharacterMovement()->bConstrainToPlane = true;
	this->GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0, 1, 0));

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);
}

void ABasePaperCharacter::BeginPlay()
{
	Super::BeginPlay();
	ProjectileSpawnPoint->SetRelativeLocation(ProjectileSpawnLoc);
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
	UE_LOG(LogTemp, Warning, TEXT("%s's health depleted!"), *this->GetName());
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

	float HurtPush = UECasts_Private::DynamicCast<APC_PlayerFox*>(this)->GetHurtPushAmount();
	if (DamageCauserLocation.X - PlayerLocation.X > 0)
	{
		HurtPush *= -1.f;
	}
	// TODO: Need an alternative to SetActorRelativeLocation; it causes player to get stuck in walls
	this->SetActorRelativeLocation(PlayerLocation + FVector(HurtPush, 0.0, 0.0));
}

void ABasePaperCharacter::DoHurt(AActor* DamageCauser)
{
	if (this->GetName().Contains("Player"))
	{
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

void ABasePaperCharacter::AIShoot()
{
	this->Shoot();
}

float ABasePaperCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                      AActor* DamageCauser)
{
	this->AddHealth(-DamageAmount);
	UE_LOG(LogTemp, Warning, TEXT("%s's health: %f"), *this->GetName(), this->GetHealth());
	
	if (this->GetHealth() <= 0)
	{
		DoDeath();
	} else {
		DoHurt(DamageCauser);
	}
	
	return DamageAmount;
}

void ABasePaperCharacter::Shoot()
{
	APC_PlayerFox* Player = UECasts_Private::DynamicCast<APC_PlayerFox*>(this);
	if (Player != nullptr)
	{
		if (Player->GetMovementComponent()->IsFalling()) return;

		int NumCherries = Player->GetCherryCount();
		if (NumCherries > 0)
		{
			NumCherries -= 1;
			UE_LOG(LogTemp, Warning, TEXT("ABasePaperCharacter::Shoot - Subtract 1 cherry; now, %s has %d charries"),
				   *Player->GetName(),
				   NumCherries
			);
			Player->SetCherryStash(NumCherries);
		} else
		{
			UE_LOG(LogTemp, Warning, TEXT("ABasePaperCharacter::Shoot - Can't shoot; %s has %d charries"),
				   *Player->GetName(),
				   NumCherries
			)
			return;
		}
	}

	AEnemyCollisionPaperCharacter* EnemyAI = UECasts_Private::DynamicCast<AEnemyCollisionPaperCharacter*>(this);
	if (EnemyAI != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ABasePaperCharacter::Shoot %s is shooting."),
		   *EnemyAI->GetName()
		);
	}
	
	float Direction = 1.f;
	if (this->GetSprite()->GetRelativeRotation().Yaw == 180.0)
	{
		Direction = -1.f;

	}
	
	UE_LOG(LogTemp, Display, TEXT("Fire!"));
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
			LogTemp, Warning, TEXT("ABasePaperCharacter::Shoot - Owner of spawned projectile, %s, is %s!"),
			*TempProjectile->GetName(),
			*TempProjectile->GetOwner()->GetName()
		);
	}
	else
	{
		UE_LOG(
			LogTemp, Warning, TEXT("ABasePaperCharacter::Shoot - %s has no projectile class set!"),
			*this->GetName()
		);
	}
}

bool ABasePaperCharacter::GetShootUpward() const
{
	return ShootUpward;
}

void ABasePaperCharacter::DestroyActor()
{
	UE_LOG(LogTemp, Warning, TEXT("Destroying %s!"), *this->GetName());
	this->Destroy();
	GetWorld()->GetTimerManager().ClearTimer(this->DeathTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(this->HurtTimerHandle);
}
