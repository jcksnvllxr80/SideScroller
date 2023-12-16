// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseInteractable.h"
#include "SideScroller/Interfaces/InteractInterface.h"
#include "Lever.generated.h"

/**
 * @class ALever
 * @brief This class represents a lever in a side-scrolling game.
 */
UCLASS()
class SIDESCROLLER_API ALever : public ABaseInteractable, public IInteractInterface
{
	GENERATED_BODY()

public:
	/**
	 * @brief Constructor for the ALever class.
	 *
	 * This method is called when an instance of the ALever class is created.
	 * It initializes the properties of the ALever object, including setting the
	 * relative scale and location of the associated InteractableBox, and enabling
	 * replication for the ALever object.
	 */
	ALever();

	/**
	 * Interacts with the object.
	 *
	 * This method is called when the player interacts with an object. The object must have a valid interaction
	 * condition in order to perform any interaction actions.
	 *
	 * @see GetCanInteract
	 * @see SetCanInteract
	 * @see ToggleLever
	 * @see UE_LOG
	 */
	UFUNCTION()
	virtual void Interact() override;

private:
	/**
	 * @brief Plays the lever move sound.
	 *
	 * This method is used to play a lever move sound. It is marked as a BlueprintCallable function,
	 * meaning it can be called from Blueprints. It is also marked as NetMulticast and Reliable,
	 * indicating that it is replicated across the network for all clients and that it is reliable
	 * and will be resent if it is lost.
	 */
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void PlayLeverMoveSound() const;

	/**
	 * @brief The array of platforms that will be triggered.
	 *
	 * This variable stores an array of AMovingPlatform objects that will be triggered when certain conditions are met.
	 * The platforms in this array can be edited in the editor using the EditAnywhere property specifier.
	 */
	UPROPERTY(EditAnywhere)
	TArray<class AMovingPlatform*> PlatformsToTrigger;

	/**
	 * \brief Toggles the state of the lever.
	 *
	 * This method is used to toggle the state of the lever. It plays the lever move sound
	 * and starts a timer to call the MoveLever method after a specified time.
	 */
	UFUNCTION(BlueprintCallable)
	void ToggleLever();

	/**
	 * TurnOffLever
	 *
	 * Turns off the lever and triggers associated platforms to stop.
	 */
	UFUNCTION()
	void TurnOffLever();

	/**
	 * Turns on the lever and triggers associated platforms.
	 *
	 * The lever's state is set to "on" and its associated flipbook is changed to the true position.
	 * Additionally, all platforms that are connected to the lever are activated.
	 */
	UFUNCTION()
	void TurnOnLever();

	/**
	 * Moves the lever.
	 *
	 * This method is called to move the lever either to the on position
	 * or the off position, depending on the current state of the lever.
	 * If the lever is currently on, it will be turned off. If it is off,
	 * it will be turned on. After moving the lever, it sets the canInteract
	 * property to true.
	 */
	UFUNCTION()
	void MoveLever();

	/**
	 * @brief Sound played when lever is moved.
	 *
	 * This variable represents the sound to be played when the lever is moved.
	 * It is an editable property that can be modified in the editor.
	 * The sound is expected to be of type USoundBase*.
	 */
	UPROPERTY(EditAnywhere)
	USoundBase* LeverMoveSound;

	/**
	 * @brief The LeverMoveTimerHandle variable holds the handle for the move timer used by the lever.
	 *
	 * The LeverMoveTimerHandle is an instance of FTimerHandle struct defined in the Unreal Engine API. It is used to
	 * keep track of the timer associated with the lever's movement. When the lever is moved, a timer is started
	 * and this handle is used to reference it. This allows for precise control over the duration and execution of
	 * the lever's movement logic
	 *.
	 *
	 * @note Make sure to initialize LeverMoveTimerHandle before using it with an actual timer handle instance. This
	 * can be done using the appropriate initialization methods provided by
	 * the Unreal Engine API.
	 * Example:
	 *     LeverMoveTimerHandle = FTimerHandle();
	 *
	 * @warning It is important to handle the destruction of the timer correctly to avoid memory leaks. Be sure to
	 * clear the timer before it is destroyed or no longer needed. This can be done using the Cancel() method
	 * provided by the Unreal Engine API.
	 * Example:
	 *     LeverMoveTimerHandle.Cancel();
	 *
	 * @see FTimerHandle
	 */
	FTimerHandle LeverMoveTimerHandle;

	/**
	 * @brief The amount of time it takes for the lever to complete one move.
	 *
	 * This variable represents the time, in seconds, it takes for the lever to complete one move from one
	 * position to another.
	 *
	 * The default value is 0.75 seconds.
	 */
	UPROPERTY(EditAnywhere)
	float LeverMoveTime = 0.75;
};
