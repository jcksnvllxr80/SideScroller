// Fill out your copyright notice in the Description page of Project Settings.


#include "Mushroom.h"

#include "Sidescroller/Characters/Players/PC_PlayerFox.h"

/**
 * \brief Default constructor for AMushroom which sets up the actor.
 */
AMushroom::AMushroom()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	this->GetPickupBox()->SetRelativeScale3D(FVector(0.2,0.2,0.2));
	this->GetPickupBox()->SetRelativeLocation(FVector(0.0,0.0,0.0));
}

/**
 * @brief Called when the game starts
 *
 * Initializes the Mushroom actor
 */
void AMushroom::BeginPlay()
{
	Super::BeginPlay();

}

/**
 * @brief This method is called every frame to update the state of the Mushroom actor.
 *
 * @param DeltaTime The time in seconds since the last frame.
 */
void AMushroom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/**
 * Gives a pickup to the specified player character.
 * This function is called when the player character overlaps with a mushroom pickup.
 * The pickup will grant healing to the player character based on the HealingValue of the mushroom.
 *
 * @param OverlappingActor The player character that is overlapping with the mushroom pickup.
 */
void AMushroom::GivePickup(APC_PlayerFox* OverlappingActor)
{
	OverlappingActor->TakeHealing(this->HealingValue);
}
