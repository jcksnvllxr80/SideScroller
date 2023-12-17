// Fill out your copyright notice in the Description page of Project Settings.


#include "FireBallProjectile.h"

/**
 * Constructor for the Fire Ball Projectile.
 *
 * Initializes the Fire Ball Projectile actor object.
 */
AFireBallProjectile::AFireBallProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

/**
 * @brief Called when the game starts or when spawned.
 *
 * This method is called when the game starts or when the projectile is spawned in the game. It is used to
 * initialize the projectile's properties and perform any necessary setup.
 */
void AFireBallProjectile::BeginPlay()
{
	Super::BeginPlay();

	this->GetProjectileFlipbook()->SetRelativeScale3D(FVector(0.750000, 0.750000, 0.750000));
	this->GetProjectileMovementComp()->ProjectileGravityScale = 0.f;
}
