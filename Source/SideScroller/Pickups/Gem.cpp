// Fill out your copyright notice in the Description page of Project Settings.


#include "Gem.h"

#include "SideScroller/Characters/Players/PC_PlayerFox.h"

/**
 * @brief Constructor for the AGem class.
 */
AGem::AGem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	this->GetPickupBox()->SetRelativeLocation(FVector(-0.52,0.0,0.48));
	this->GetPickupBox()->SetRelativeScale3D(FVector(0.200000,0.200000,0.168750));
}

/**
 * Begins playing the game.
 *
 * @param None
 *
 * @return None
 */
void AGem::BeginPlay()
{
	Super::BeginPlay();
}

/**
 * @brief Performs per-frame updates for the AGem class.
 *
 * This method is called every frame to update the AGem object.
 *
 * @param DeltaTime The time (in seconds) since the last frame.
 *
 * @see Super::Tick(float DeltaTime)
 */
void AGem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/**
 * GivePickup method in the AGem class.
 *
 * This method is used to give a pickup (in this case, a gem) to a player character.
 * It checks if the provided overlapping actor is valid, and takes the monetary value of the gem from
 * the player character.
 *
 * @param OverlappingActor The player character that has overlapping with the gem to be picked up.
 */
void AGem::GivePickup(APC_PlayerFox* OverlappingActor)
{
	if (OverlappingActor == nullptr) return;
	
	OverlappingActor->TakeMoney(this->MonetaryValue);
}
