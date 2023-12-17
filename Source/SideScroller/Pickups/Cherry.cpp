// Fill out your copyright notice in the Description page of Project Settings.


#include "Cherry.h"

#include "SideScroller/Characters/Players/PC_PlayerFox.h"

/**
 * \brief Default constructor for AACherry class
 *
 * This method is the default constructor for the AACherry class. It initializes the object with default values.
 */
AACherry::AACherry()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	this->GetPickupBox()->SetRelativeScale3D(FVector(0.2,0.2,0.2));
	this->GetPickupBox()->SetRelativeLocation(FVector(0.0,0.0,0.0));
}

/**
 * @brief The BeginPlay method
 *
 * This method is called when the game starts or when the actor is spawned into the world. It should be used for
 * any initialization that needs to be done before the game starts.
 */
void AACherry::BeginPlay()
{
	Super::BeginPlay();

}

/**
 * @brief Executes every frame.
 *
 * This method is called every frame to update the state of the AACherry object.
 *
 * @param DeltaTime The time since the last frame update, in seconds.
 *
 * @note This method overrides the Tick method in the parent class.
 */
void AACherry::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/**
 * Gives the pickup to the specified player character.
 *
 * @param OverlappingActor The player character to give the pickup to.
 */
void AACherry::GivePickup(APC_PlayerFox* OverlappingActor)
{
	OverlappingActor->TakeCherries(this->CherryAmount);
}

