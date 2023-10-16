// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_EnemyFrog.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

APC_EnemyFrog::APC_EnemyFrog()
{
	PrimaryActorTick.bCanEverTick = false;

	this->GetDamageBox()->SetRelativeScale3D(FVector(-0.100219,0.120000,0.252439));
	
	this->GetLeftHurtBox()->SetRelativeScale3D(FVector(0.125698,0.161768,0.179930));
	this->GetLeftHurtBox()->SetRelativeLocation(FVector(-7.000000,0.000000,-1.000000));
	
	this->GetRightHurtBox()->SetRelativeScale3D(FVector(0.125698,0.161768,0.179930));
	this->GetRightHurtBox()->SetRelativeLocation(FVector(7.000000,0.000000,-1.000000));

}

void APC_EnemyFrog::BeginPlay()
{
	Super::BeginPlay();

}

void APC_EnemyFrog::SetProjectileTransform(
	const float Direction,
	AActor* MyOwner,
	const ABasePaperCharacter* BaseChar,
	const ABaseProjectile* Projectile
) {
	// Projectile->GetProjectileFlipbook()->SetRelativeRotation(BaseChar->GetArrowComponent()->GetComponentRotation());
	Projectile->GetProjectileFlipbook()->SetRelativeRotation(BaseChar->GetActorRotation());
	Projectile->GetProjectileMovementComp()->Velocity = FVector(
		Direction * Projectile->GetMovementSpeed(), 0.f, 0.f
	);
}
