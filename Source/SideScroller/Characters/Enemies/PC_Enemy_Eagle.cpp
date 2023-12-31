// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Enemy_Eagle.h"

#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

/**
 * Constructor for APC_Enemy_Eagle.
 */
APC_Enemy_Eagle::APC_Enemy_Eagle()
{
	PrimaryActorTick.bCanEverTick = false;
	
	this->GetDamageBox()->SetRelativeLocation(FVector(-0.873273,0.000000,2.947091));
	this->GetDamageBox()->SetRelativeScale3D(FVector(0.168750,0.463882,0.365065));

	this->GetLeftHurtBox()->SetRelativeLocation(FVector(-10.564844,-0.000000,1.837726));
	this->GetLeftHurtBox()->SetRelativeScale3D(FVector(0.100698,0.161768,0.211180));

	this->GetRightHurtBox()->SetRelativeLocation(FVector(7.632283,-0.000000,2.335176));
	this->GetRightHurtBox()->SetRelativeScale3D(FVector(0.125698,0.161768,0.242430));

	this->GetCharacterMovement()->GetNavAgentPropertiesRef().bCanFly = true;
	this->SetDamage(20.0);
}

/**
 * @brief Function called when the enemy eagle starts playing.
 *
 * This function is called when the enemy eagle character starts playing. It sets the movement
 * mode to flying and adjusts the maximum fly speed to 50.0 units per second.
 *
 * @param None
 * @return None
 */
void APC_Enemy_Eagle::BeginPlay()
{
	Super::BeginPlay();
	
	this->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	this->GetCharacterMovement()->MaxFlySpeed = 50.f;
}

/**
 * Sets the transform of the projectile based on the given parameters.
 *
 * @param Direction The direction in which the projectile should be launched.
 * @param MyOwner The actor that owns the projectile.
 * @param BaseChar The base character that the projectile is targeting.
 * @param Projectile The projectile to set the transform for.
 */
void APC_Enemy_Eagle::SetProjectileTransform(
	const float Direction,
	AActor* MyOwner,
	const ABasePaperCharacter* BaseChar,
	const ABaseProjectile* Projectile
) {
	FRotator OwnerRotation;
	const float ProjectilePitchRads = GetEnemyToPlayerPitchRadians(BaseChar, OwnerRotation, Direction);
	
	Projectile->GetProjectileFlipbook()->SetRelativeRotation(OwnerRotation);
	Projectile->GetProjectileMovementComp()->Velocity = FVector(
		Projectile->GetMovementSpeed() * Direction * cos(ProjectilePitchRads),
		0.f,
		Projectile->GetMovementSpeed() * Direction * sin(ProjectilePitchRads)
	);
}

/**
 * Calculates the pitch angle in radians between an enemy character and the player.
 *
 * @param BaseChar The enemy character.
 * @param OwnerRotation The rotation of the enemy character.
 * @param Direction The direction of the pitch angle.
 * @return The pitch angle in radians between the enemy character and the player.
 */
float APC_Enemy_Eagle::GetEnemyToPlayerPitchRadians(
	const ABasePaperCharacter* BaseChar,
	FRotator& OwnerRotation,
	float Direction
) const {
	OwnerRotation = BaseChar->GetActorRotation();

	// UE_LOG(LogTemp, VeryVerbose,
	// 	TEXT("ABaseProjectile::GetEnemyPlayerPitch - %s's (owner) rotation is %s."),
	// 	*BaseChar->GetName(), *OwnerRotation.ToString()
	// );

	const APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (PlayerPawn == nullptr) return 0.0;  // player not in existence anymore, return 0.0 radians

	const FVector PlayerLocation = PlayerPawn->GetTargetLocation();
	// UE_LOG(LogTemp, VeryVerbose,
	// 	TEXT("ABaseProjectile::GetEnemyPlayerPitch - %s's (player) location is %s."),
	// 	*GetWorld()->GetFirstPlayerController()->GetName(), *PlayerLocation.ToString()
	// );
	
	const FVector OwnerLocation = BaseChar->GetActorLocation();
	// UE_LOG(LogTemp, VeryVerbose,
	// 	TEXT("ABaseProjectile::GetEnemyPlayerPitch - %s's (owner) location is %s."),
	// 	*BaseChar->GetName(), *OwnerLocation.ToString()
	// );
	
	const FVector ProjectileVector = PlayerLocation - OwnerLocation;
	// UE_LOG(LogTemp, VeryVerbose,
	// 	TEXT("ABaseProjectile::GetEnemyPlayerPitch - projectile vector should be %s."),
	// 	*ProjectileVector.ToString()
	// );

	const float DzDx = ProjectileVector.Z / ProjectileVector.X;
	// UE_LOG(LogTemp, VeryVerbose, TEXT("ABaseProjectile::GetEnemyPlayerPitch - DzDx should be %f."), DzDx);

	const float PitchRadians = atan(DzDx);
	// UE_LOG(LogTemp, VeryVerbose, TEXT("ABaseProjectile::GetEnemyPlayerPitch - PitchRadians should be %f."), PitchRadians);
	
	const float ProjectilePitch = Direction * (180 * PitchRadians / PI);
	// UE_LOG(LogTemp, VeryVerbose, TEXT("ABaseProjectile::GetEnemyPlayerPitch - PitchDegrees should be %f."), ProjectilePitch);
	
	OwnerRotation = FRotator(ProjectilePitch, OwnerRotation.Yaw, OwnerRotation.Roll);
	// UE_LOG(LogTemp, VeryVerbose,
	// 	TEXT("ABaseProjectile::GetEnemyPlayerPitch - Rotation from owner to player is %s."),
	// 	*OwnerRotation.ToString()
	// );
	return PitchRadians;
}