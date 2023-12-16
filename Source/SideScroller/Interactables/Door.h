// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseInteractable.h"
#include "SideScroller/Interfaces/InteractInterface.h"
#include "Door.generated.h"

/**
 * @class ADoor
 * @brief Represents a door in the side scroller game.
 *
 * The ADoor class is responsible for managing the state and behavior of a door in the side scroller game.
 * It inherits from ABaseInteractable and implements the IInteractInterface.
 *
 * @see ABaseInteractable
 * @see IInteractInterface
 */
UCLASS()
class SIDESCROLLER_API ADoor : public ABaseInteractable, public IInteractInterface
{
	GENERATED_BODY()

public:
	/**
	 * @brief Constructor for the ADoor class.
	 *
	 * This constructor initializes the InteractableBox component of the ADoor class.
	 * It sets the relative scale of the InteractableBox to (0.5, 0.4, 0.5) and the relative location to
	 * (0.0, 0.0, 0.0).
	 */
	ADoor();

	/**
	 * @brief Performs an interaction with the door.
	 *
	 * This method is responsible for performing an interaction with the door.
	 * If the door can be interacted with, it toggles the door state.
	 *
	 * @details
	 * The interaction can only be performed if the door can be interacted with.
	 * After performing the interaction, the door's `CanInteract` flag is set to false
	 * to prevent multiple interactions from happening simultaneously.
	 *
	 * @see SetCanInteract
	 * @see GetCanInteract
	 * @see ToggleDoor
	 *
	 * @note This method is intended to be called as a blueprint callable function.
	 *
	 * @warning This method should only be called on `ADoor` instances.
	 *
	 * @pre None
	 * @post The door state may change if the interaction was successful.
	 *
	 * @exception None
	 *
	 * @param None
	 *
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Interact() override;

	/**
	 * Closes the door, plays the door close sound, and sets a timer to close the door automatically.
	 *
	 * This method is callable from Blueprint.
	 *
	 * @param None
	 *
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	void CloseDoorSoundAndTimer();

	/**
	 * @brief Plays the door open sound and sets a timer to automatically close the door.
	 *
	 * This method is a blueprint callable function that can be used to open a door in the game.
	 * It plays the specified door open sound and sets a timer to automatically close the door after a certain
	 * amount of time.
	 *
	 * @note This method assumes that the `PlayDoorSound` function and `GetWorld()->GetTimerManager()` are
	 * already available.
	 *
	 * @param None
	 *
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	void OpenDoorSoundAndTimer();

	/**
	 *
	 */
	UFUNCTION(BlueprintCallable)
	bool GetIsOpen() const;

private:
	/**
	 * @brief Toggles the state of the door.
	 *
	 * This method is used to toggle the state of the door. If the door is currently
	 * open, it will close the door by calling the CloseDoorSoundAndTimer method.
	 * If the door is currently closed, it will open the door by calling the OpenDoorSoundAndTimer method.
	 *
	 * @param None.
	 *
	 * @return None.
	 */
	UFUNCTION(BlueprintCallable)
	void ToggleDoor();

	/**
	 * \brief Closes the door and sets the door state to closed.
	 *
	 * This method is used to close the door and update the door state to closed. The update includes setting the
	 * bIsTrue variable to false, setting the InteractableFlipbook's flipbook to the FalsePosition, and enabling
	 * interaction by setting SetCanInteract to true.
	 */
	UFUNCTION(BlueprintCallable)
	void CloseDoor();

	/**
	 * @brief Opens the door by setting the door to open and changing the flipbook to the true position.
	 *
	 * This function is a BlueprintCallable function that can be used to open the door. It sets the door to open,
	 * changes the flipbook to the true position, and enables interaction with the door.
	 */
	UFUNCTION(BlueprintCallable)
	void OpenDoor();

	/**
	 * \brief Play a sound when a door is opened or closed.
	 *
	 * This function is a BlueprintCallable function, which means it can be
	 * accessed from Blueprints. It is also a NetMulticast function, which means
	 * it is called on both the server and all connected clients. It is reliable, meaning that
	 * the function will be resent if it gets lost during network transmission.
	 *
	 * \param DoorSound The sound to be played when the door is opened or closed.
	 *
	 * \remarks This function does not return anything. It simply plays the specified sound when called.
	 */
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void PlayDoorSound(USoundBase* DoorSound) const;

	/**
	 * @brief The sound that plays when the door is opened.
	 *
	 * The DoorOpenSound variable stores a reference to the sound that should be played when the door is opened.
	 * This sound can be set and accessed from the UPROPERTY macros and will be editable in the Unreal Engine editor.
	 * To refer to the actual sound asset, use the USoundBase class.
	 */
	UPROPERTY(EditAnywhere)
	USoundBase* DoorOpenSound;

	/**
	 * @brief DoorCloseSound variable
	 *
	 * This variable represents the sound that will be played when a door closes.
	 * It is of type USoundBase* and can be edited anywhere.
	 */
	UPROPERTY(EditAnywhere)
	USoundBase* DoorCloseSound;

	/**
	 * @var DoorOpenTimerHandle
	 * @brief Variable for tracking the timer handle used for opening doors.
	 *
	 * This variable of type FTimerHandle is used to track the timer handle associated
	 * with the opening of doors. The timer handle can be used to start, stop, or check
	 * the status of a timer responsible for opening doors in a game or application.
	 */
	FTimerHandle DoorOpenTimerHandle;

	/**
	 * @brief Handle for the timer used to close the door.
	 *
	 * This variable represents the timer handle used to manage the door closing functionality.
	 * The timer handle is used to keep track of the timer associated with closing the door.
	 * It allows starting, stopping, or resetting the timer as needed.
	 */
	FTimerHandle DoorCloseTimerHandle;

	/**
	 * @brief The time it takes for a door to remain open after being opened.
	 *
	 * The DoorOpenTime variable represents the duration in seconds that a door remains open after being opened.
	 *
	 * The value of this variable can be edited in the Unreal Engine Editor, specifically in the "EditAnywhere"
	 * category.
	 *
	 * For example, a DoorOpenTime value of 0.75 indicates that the door will remain open for 0.75 seconds after it
	 * has been opened.
	 *
	 * @warning Modifying this value without considering the gameplay or level design may result in
	 * unintended consequences.
	 */
	UPROPERTY(EditAnywhere)
	float DoorOpenTime = 0.75;

	/**
	 * @brief DoorCloseTime is a variable that represents the time it takes for a door to close.
	 *
	 * This variable is used in the context of door systems,
	 * where it determines the duration of time it takes for a door to close completely after it has been triggered.
	 *
	 * The value of DoorCloseTime is a floating point number,
	 * expressed in seconds.
	 *
	 * By default, the value of DoorCloseTime is set to 0.25 seconds.
	 *
	 * @note This variable can be modified in the editor using the EditAnywhere specifier.
	 *
	 * @see DoorSystem
	 *
	 */
	UPROPERTY(EditAnywhere)
	float DoorCloseTime = 0.25;

	/**
	 * @brief The scale of the flipbook.
	 */
	UPROPERTY(VisibleAnywhere)
	FVector FlipbookScale;
};
