// Fill out your copyright notice in the Description page of Project Settings.


#include "FireBallProjectile.h"

AFireBallProjectile::AFireBallProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AFireBallProjectile::BeginPlay()
{
	Super::BeginPlay();

	this->GetProjectileFlipbook()->SetRelativeScale3D(FVector(0.750000, 0.750000, 0.750000));
	this->GetProjectileMovementComp()->ProjectileGravityScale = 0.f;
}
