// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

#include "PaperSpriteComponent.h"
#include "Net/UnrealNetwork.h"

/**
 * @brief Constructor for the AMovingPlatform class.
 *
 * Initializes the PrimaryActorTick and sets the mobility of the render component to movable.
 * The actor is set to replicate across network.
 */
AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	GetRenderComponent()->SetMobility(EComponentMobility::Movable);

	this->SetReplicates(true);
}

/**
 * @brief Called when the game starts or when spawned.
 *
 * This function is called when the actor begins play. It is responsible for setting up initial properties
 * and state for the actor.
 */
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	SetReplicates(true);
	if (HasAuthority())
	{
		SetReplicateMovement(true);
	}
	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

/**
 * @brief Update the platform's position.
 *
 * This method is responsible for updating the position of the moving platform. It is typically called every frame
 * to continuously move the platform according to its speed and target locations.
 *
 * @param DeltaTime The time elapsed since the last frame.
 */
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ActiveTriggers > 0)
	{
		if (HasAuthority())
		{
			FVector Location = GetActorLocation();
			const float JourneyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
			const float LengthTraveled = (Location - GlobalStartLocation).Size();
			if (LengthTraveled >= JourneyLength)
			{
				const FVector Swap = GlobalStartLocation;
				GlobalStartLocation = GlobalTargetLocation;
				GlobalTargetLocation = Swap;
			}
			Location += Speed * DeltaTime * (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
			SetActorLocation(Location);
		}
	}
}

/**
 * @brief Increments the ActiveTriggers variable by 1.
 *
 * This method is used to add an active trigger to the moving platform. Each active trigger increments the
 * ActiveTriggers variable by 1.
 */
void AMovingPlatform::AddActiveTrigger()
{
	ActiveTriggers++;
}

/**
 * @brief Remove one active trigger from the Moving Platform
 *
 * Decreases the number of active triggers by 1. If there are no active triggers, the count remains unchanged.
 */
void AMovingPlatform::RemoveActiveTrigger()
{
	if (ActiveTriggers > 0)
	{
		ActiveTriggers--;
	}
}

/**
 * Retrieves the replicated properties for the lifetime of the actor.
 *
 * This method overrides the base implementation of GetLifetimeReplicatedProps defined in the Actor class.
 * It should be used to define which actor properties are replicated across the network during the actor's lifetime.
 *
 * @param OutLifetimeProps A reference to a TArray<FLifetimeProperty> object that will be populated with the
 * replicated properties.
 * @see Super::GetLifetimeReplicatedProps
 * @see DOREPLIFETIME
 */
void AMovingPlatform::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMovingPlatform, ActiveTriggers);
}
