// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_EnemyFrog.h"

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
