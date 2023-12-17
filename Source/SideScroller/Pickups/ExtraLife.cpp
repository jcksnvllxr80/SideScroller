// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtraLife.h"

#include "SideScroller/Characters/Players/PC_PlayerFox.h"

/**
 * Constructor for AExtraLife class.
 * Sets up the actor to call Tick() every frame and initialize the pickup box properties.
 */
AExtraLife::AExtraLife()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	this->GetPickupBox()->SetRelativeScale3D(FVector(0.2,0.2,0.2));
	this->GetPickupBox()->SetRelativeLocation(FVector(0.0,0.0,0.0));
}

/**
 * @brief Called when the game starts or when spawned.
 *
 * This method is called when the game starts or when the actor is spawned in the world. It is typically used
 * for initialization tasks that need to be performed once, such as setting up variables, configuring components,
 * or registering delegates. It is automatically called by the game engine and should not be called directly.
 */
void AExtraLife::BeginPlay()
{
	Super::BeginPlay();

}

/**
 * @brief Update the logic of the AExtraLife actor every frame.
 *
 * This method is called every frame to update the logic of the AExtraLife actor.
 * It calls the parent class's Tick method with the specified DeltaTime.
 *
 * @param DeltaTime The time elapsed since the last frame.
 */
void AExtraLife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/**
 * Gives a pickup to the specified player actor.
 *
 * This method increases the number of lives of the given player actor by 1.
 *
 * @param OverlappingActor The player actor to give the pickup to.
 */
void AExtraLife::GivePickup(APC_PlayerFox* OverlappingActor)
{
	OverlappingActor->SetNumberOfLives(OverlappingActor->GetNumberOfLives() + 1);
}
