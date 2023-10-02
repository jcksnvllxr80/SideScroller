// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Enemy_Opossum.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

APC_Enemy_Opossum::APC_Enemy_Opossum()
{

	PrimaryActorTick.bCanEverTick = false;

	this->GetDamageBox()->SetRelativeLocation(FVector(0.000000, 0.000000, 3.000000));
	this->GetDamageBox()->SetRelativeScale3D(FVector(-0.100219,0.120000,0.25));

	this->GetLeftHurtBox()->SetRelativeLocation(FVector(8.000000,0.000000,2.000000));
	this->GetLeftHurtBox()->SetRelativeScale3D(FVector(0.125698,0.161768,0.117430));

	this->GetRightHurtBox()->SetRelativeLocation(FVector(-10.000000,0.000000,1.75000));
	this->GetRightHurtBox()->SetRelativeScale3D(FVector(0.188198,0.161768,0.055));

	this->SetDamage(30.f);
}

void APC_Enemy_Opossum::BeginPlay()
{
	Super::BeginPlay();
}

void APC_Enemy_Opossum::SetProjectileTransform(
	const float Direction,
	AActor* MyOwner,
	const ABasePaperCharacter* BaseChar,
	const ABaseProjectile* Projectile
) {
	Projectile->GetProjectileFlipbook()->SetRelativeRotation(BaseChar->GetArrowComponent()->GetComponentRotation());
	Projectile->GetProjectileMovementComp()->Velocity = FVector(
		Direction * Projectile->GetMovementSpeed(), 0.f, 0.f
	);
}
