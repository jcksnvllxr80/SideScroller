// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "Kismet/GameplayStatics.h"

/**
 * @brief Constructor for the ADoor class.
 *
 * This method initializes the InteractableBox scale and location.
 */
ADoor::ADoor()
{
	InteractableBox->SetRelativeScale3D(FVector(0.5,0.4,0.5));
	InteractableBox->SetRelativeLocation(FVector(0.0,0.0,0.0));
}

/**
 * @brief Method to interact with the door
 *
 * This method is used to interact with the door and toggle its state.
 */
void ADoor::Interact()
{
	if (GetCanInteract())
	{
		SetCanInteract(false);
		ToggleDoor();
	}
}

/**
 * Toggles the state of the door.
 *
 * @param bIsTrue  The boolean value indicating whether the door is already open or closed.
 */
void ADoor::ToggleDoor()
{
	if (bIsTrue)
	{
		CloseDoorSoundAndTimer();
	}
	else
	{
		OpenDoorSoundAndTimer();
	}
}

/**
 * @brief Close the door and set it to the closed position.
 *
 * This method sets the door to the closed position by updating the flipbook and setting
 * the interactivity of the door to true.
 */
void ADoor::CloseDoor()
{
	UE_LOG(LogTemp, Display, TEXT("ADoor::CloseDoor - Setting door to closed"))
	bIsTrue = false;
	InteractableFlipbook->SetFlipbook(FalsePosition);

	SetCanInteract(true);
}

/**
 * @brief Open the door.
 *
 * This method sets the door to open by performing the following actions:
 * - Prints a log message using UE_LOG to indicate that the door is being set to open.
 * - Sets the bIsTrue variable to true indicating that the door is open.
 * - Sets the InteractableFlipbook's flipbook to TruePosition, which represents the open position of the door.
 * - Enables interaction with the door by setting the CanInteract variable to true.
 */
void ADoor::OpenDoor()
{
	UE_LOG(LogTemp, Display, TEXT("ADoor::OpenDoor - Setting door to open"))
	bIsTrue = true;
	InteractableFlipbook->SetFlipbook(TruePosition);

	SetCanInteract(true);
}

/**
 * Plays the specified door sound when the door is interacted with.
 *
 * @param DoorSound The sound to play.
 * @return void
 */
void ADoor::PlayDoorSound_Implementation(USoundBase* DoorSound) const
{
	UGameplayStatics::SpawnSoundAttached(
		DoorSound,
		this->InteractableFlipbook,
		TEXT("DoorSound")
	);
}

/**
 * @brief Plays the door close sound and starts a timer to close the door after a delay.
 *
 * This method plays the door close sound using the provided sound asset and starts a timer to automatically
 * close the door after a specified duration.
 *
 * @param DoorCloseSound The sound asset to be played when the door is closing.
 * @return None.
 */
void ADoor::CloseDoorSoundAndTimer()
{
	PlayDoorSound(DoorCloseSound);

	GetWorld()->GetTimerManager().SetTimer(
		this->DoorCloseTimerHandle,
		this,
		&ADoor::CloseDoor,
		DoorCloseTime,
		false
	);
}

/**
 * @brief Plays the door open sound and sets a timer to open the door.
 *
 * This method plays the door open sound and sets a timer to call the OpenDoor method after a specified duration.
 *
 * @param DoorOpenSound The sound to play when the door opens.
 * @param DoorOpenTime The duration after which the door should open.
 *
 * @return None.
 */
void ADoor::OpenDoorSoundAndTimer()
{
	PlayDoorSound(DoorOpenSound);

	GetWorld()->GetTimerManager().SetTimer(
		this->DoorOpenTimerHandle,
		this,
		&ADoor::OpenDoor,
		DoorOpenTime,
		false
	);
}

/**
 * @brief Get the state of the door, whether it is open or not.
 *
 * @return True if the door is open, false otherwise.
 */
bool ADoor::GetIsOpen() const
{
	return bIsTrue;
}
