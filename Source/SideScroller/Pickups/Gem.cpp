// Fill out your copyright notice in the Description page of Project Settings.


#include "Gem.h"

#include "SideScroller/Characters/Players/PC_PlayerFox.h"

AGem::AGem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	this->GetPickupBox()->SetRelativeLocation(FVector(-0.52,0.0,0.48));
	this->GetPickupBox()->SetRelativeScale3D(FVector(0.200000,0.200000,0.168750));
}

void AGem::BeginPlay()
{
	Super::BeginPlay();
}

void AGem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGem::GivePickup(APC_PlayerFox* OverlappingActor)
{
	if (OverlappingActor == nullptr) return;
	
	OverlappingActor->TakeMoney(this->MonetaryValue);
}
