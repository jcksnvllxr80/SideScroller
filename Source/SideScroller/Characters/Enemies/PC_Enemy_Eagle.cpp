// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Enemy_Eagle.h"

#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APC_Enemy_Eagle::APC_Enemy_Eagle()
{
	PrimaryActorTick.bCanEverTick = false;
	
	this->GetDamageBox()->SetRelativeLocation(FVector(-1.600000, 0.000000, 7.800000));
	this->GetDamageBox()->SetRelativeScale3D(FVector(0.2,0.12,0.12));

	this->GetLeftHurtBox()->SetRelativeLocation(FVector(-7.000000,0.000000,-4.000000));
	this->GetLeftHurtBox()->SetRelativeScale3D(FVector(0.225698,0.161768,0.211180));

	this->GetRightHurtBox()->SetRelativeLocation(FVector(8.000000,0.000000,-1.000000));
	this->GetRightHurtBox()->SetRelativeScale3D(FVector(0.125698,0.161768,0.179930));

	this->SetDamage(20.0);
}

void APC_Enemy_Eagle::BeginPlay()
{
	Super::BeginPlay();

	this->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
}
