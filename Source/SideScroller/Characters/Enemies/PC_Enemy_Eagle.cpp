// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Enemy_Eagle.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APC_Enemy_Eagle::APC_Enemy_Eagle()
{
	PrimaryActorTick.bCanEverTick = false;
	
	this->GetDamageBox()->SetRelativeLocation(FVector(1.600000, 0.000000, 7.800000));
	this->GetDamageBox()->SetRelativeScale3D(FVector(0.2,0.12,0.12));

	this->GetLeftHurtBox()->SetRelativeLocation(FVector(7.000000,0.000000,-4.000000));
	this->GetLeftHurtBox()->SetRelativeScale3D(FVector(0.225698,0.161768,0.211180));

	this->GetRightHurtBox()->SetRelativeLocation(FVector(-8.000000,0.000000,-1.000000));
	this->GetRightHurtBox()->SetRelativeScale3D(FVector(0.125698,0.161768,0.179930));

	this->SetDamage(20.0);
}

void APC_Enemy_Eagle::BeginPlay()
{
	Super::BeginPlay();
	
	this->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
}

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

float APC_Enemy_Eagle::GetEnemyToPlayerPitchRadians(
	const ABasePaperCharacter* BaseChar,
	FRotator& OwnerRotation,
	float Direction
) const {
	OwnerRotation = BaseChar->GetArrowComponent()->GetComponentRotation();
	// UE_LOG(LogTemp, Warning,
	// 	TEXT("ABaseProjectile::GetEnemyPlayerPitch - %s's (owner) rotation is %s."),
	// 	*BaseChar->GetName(), *OwnerRotation.ToString()
	// );

	const FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetTargetLocation();
	// UE_LOG(LogTemp, Warning,
	// 	TEXT("ABaseProjectile::GetEnemyPlayerPitch - %s's (player) location is %s."),
	// 	*GetWorld()->GetFirstPlayerController()->GetName(), *PlayerLocation.ToString()
	// );
	
	const FVector OwnerLocation = BaseChar->GetActorLocation();
	// UE_LOG(LogTemp, Warning,
	// 	TEXT("ABaseProjectile::GetEnemyPlayerPitch - %s's (owner) location is %s."),
	// 	*BaseChar->GetName(), *OwnerLocation.ToString()
	// );
	
	const FVector ProjectileVector = PlayerLocation - OwnerLocation;
	// UE_LOG(LogTemp, Warning,
	// 	TEXT("ABaseProjectile::GetEnemyPlayerPitch - projectile vector should be %s."),
	// 	*ProjectileVector.ToString()
	// );

	const float DzDx = ProjectileVector.Z / ProjectileVector.X;
	// UE_LOG(LogTemp, Warning, TEXT("ABaseProjectile::GetEnemyPlayerPitch - DzDx should be %f."), DzDx);

	const float PitchRadians = atan(DzDx);
	// UE_LOG(LogTemp, Warning, TEXT("ABaseProjectile::GetEnemyPlayerPitch - PitchRadians should be %f."), PitchRadians);
	
	const float ProjectilePitch = Direction * (180 * PitchRadians / PI);
	// UE_LOG(LogTemp, Warning, TEXT("ABaseProjectile::GetEnemyPlayerPitch - PitchDegrees should be %f."), ProjectilePitch);
	
	OwnerRotation = FRotator(ProjectilePitch, OwnerRotation.Yaw, OwnerRotation.Roll);
	// UE_LOG(LogTemp, Warning,
	// 	TEXT("ABaseProjectile::GetEnemyPlayerPitch - Rotation from owner to player is %s."),
	// 	*OwnerRotation.ToString()
	// );
	return PitchRadians;
}