// Fill out your copyright notice in the Description page of Project Settings.


#include "Lever.h"

#include "Kismet/GameplayStatics.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"
#include "SideScroller/Mechanics/PlatformBlocks/MovingPlatform.h"

/**
 * @brief Constructor for the ALever class.
 *
 * This method initializes the ALever instance by setting the relative scale and location of the InteractableBox component. It also enables replication for the ALever instance.
 *
 * @param None.
 *
 * @return None.
 */
ALever::ALever()
{
	InteractableBox->SetRelativeScale3D(FVector(0.5,0.4,0.2));
	InteractableBox->SetRelativeLocation(FVector(0.0,0.0,0.0));
	this->SetReplicates(true);
}

/**
 * @brief Allows a player to interact with the lever.
 *
 * When called, this method checks if the lever can currently be interacted with.
 * If it can, the method sets the canInteract flag to false and toggles the lever's state.
 * If the lever cannot be interacted with, a log message is displayed indicating that the lever is not ready to be interacted with.
 *
 * @param None.
 * @return None.
 */
void ALever::Interact()
{
	if (GetCanInteract())
	{
		SetCanInteract(false);
		ToggleLever();
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("ALever::Interact - Lever not ready to interact."))
}

/**
 * Turn off the lever.
 *
 * This method sets the lever to the off position by updating the value of bIsTrue to false.
 * It also updates the flipbook of the interactable to display the false position.
 * Additionally, it removes the active trigger from all the platforms that are set to be triggered by this lever.
 */
void ALever::TurnOffLever()
{
	UE_LOG(LogTemp, Display, TEXT("ALever::ToggleLever - Setting lever to off"))
	bIsTrue = false;
	InteractableFlipbook->SetFlipbook(FalsePosition);

	for (AMovingPlatform* Platform: PlatformsToTrigger)
	{
		Platform->RemoveActiveTrigger();
	}
}

/**
 * Turn on the lever.
 *
 * This method sets the lever to the "on" state, displays a log message, updates the flipbook to the
 * true position, and adds an active trigger to each platform specified in the PlatformsToTrigger array.
 */
void ALever::TurnOnLever()
{
	UE_LOG(LogTemp, Display, TEXT("ALever::ToggleLever - Setting lever to on"))
	bIsTrue = true;
	InteractableFlipbook->SetFlipbook(TruePosition);
	
	for (AMovingPlatform* Platform: PlatformsToTrigger)
	{
		Platform->AddActiveTrigger();
	}
}

/**
 * Moves the lever by either turning it on or off, and sets the ability to interact with it.
 *
 * @param bIsTrue Boolean indicating whether the lever should be turned on or off.
 */
void ALever::MoveLever()
{
	if (bIsTrue)
	{
		TurnOffLever();
	}
	else
	{
		TurnOnLever();
	}

	SetCanInteract(true);
}

/**
 * @brief Plays the lever move sound.
 *
 * This method is used to play the lever move sound effect when the lever is activated.
 * It spawns the sound attached to the lever's interactable flipbook.
 */
void ALever::PlayLeverMoveSound_Implementation() const
{
	UGameplayStatics::SpawnSoundAttached(
		this->LeverMoveSound,
		this->InteractableFlipbook,
		TEXT("LeverMoveSound")
	);
}

/**
 * Toggles the lever by playing the move sound and setting a timer to move the lever.
 *
 * @param LeverMoveTime The time it takes for the lever to move.
 */
void ALever::ToggleLever()
{
	PlayLeverMoveSound();

	GetWorld()->GetTimerManager().SetTimer(
		this->LeverMoveTimerHandle,
		this,
		&ALever::MoveLever,
		LeverMoveTime,
		false
	);
}
