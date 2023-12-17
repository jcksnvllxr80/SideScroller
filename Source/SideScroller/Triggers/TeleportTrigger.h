// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"
#include "TeleportTrigger.generated.h"

/**
 * \class ADoor
 * \brief Represents a door in the game.
 *
 * The ADoor class is responsible for managing the state and behavior of a door in the game. It inherits from
 * ABaseInteractable and implements the IInteractInterface for interaction functionality. This class provides
 * methods for opening, closing, and toggling the door state, as well as playing sound effects and managing
 * timers for automatic door closure.
 *
 * \note This class assumes that Unreal Engine and the necessary engine components are available.
 *
 * \attention This class should only be used on instances of ADoor.
 */
class ADoor;
/**
 * \class ATeleportTrigger
 *
 * \brief ATriggerBox subclass that teleports the player when overlapped.
 *
 * This class represents a trigger box in the game world that triggers a teleportation
 * when the player character overlaps with it. The class inherits from ATriggerBox, a
 * built-in Unreal Engine class, and adds functionality to handle teleportation logic.
 * It provides methods to teleport the player, play a teleport sound, and prepare for teleportation.
 * It also includes properties to set the target location, teleport delay timer, teleport source door,
 * global teleport target location, and teleport sound.
 *
 * To use this class, create an instance of it in the game level and set the desired properties.
 * Handle the Begin Play event to initialize any necessary logic. The Teleport method can be called
 * to execute the teleportation for a specific player character. The NotifyActorBeginOverlap method
 * is called automatically when another actor overlaps with the trigger, and the NotifyActorEndOverlap
 * method is called when the overlapping ends.
 *
 * This class is Blueprintable, allowing Blueprint scripters to create instances of it and modify its properties.
 */
UCLASS()
class SIDESCROLLER_API ATeleportTrigger : public ATriggerBox
{
	GENERATED_BODY()

public:
	/**
	 * @brief ATeleportTrigger constructor.
	 *
	 * This constructor initializes the ATeleportTrigger object.
	 *
	 * @param None.
	 *
	 * @return None.
	 */
	ATeleportTrigger();

	/**
	 * \brief Teleports a player character to a specified location.
	 *
	 * This method teleports the given player character to a global teleport target location.
	 *
	 * \param Player The player character to teleport.
	 */
	UFUNCTION(BlueprintCallable)
	void Teleport(APC_PlayerFox* Player);

	/**
	 * @brief The TeleportTargetLocation variable represents the target location for teleportation.
	 *
	 * This variable is of type FVector and is editable anywhere in the game editor. It has a metadata
	 * tag (MakeEditWidget = true) which allows for easier editing using a widget in the editor.
	 *
	 * @see FVector
	 */
	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	FVector TeleportTargetLocation;

	/**
	 * @brief Initializes gameplay for this actor.
	 *
	 * This method is called when the game begins or when this actor is placed in the world.
	 * It is responsible for initializing any gameplay-related functionality, such as setting up variables,
	 * registering input bindings, and preparing the actor for gameplay.
	 *
	 * @param None
	 *
	 * @return None
	 *
	 * @note This method should be overridden in child classes to implement customized behavior.
	 */
	virtual void BeginPlay() override;

	/**
	 * Called every frame.
	 *
	 * This method is called every frame to update the Teleport Trigger.
	 * If the player is overlapping the trigger, the door is open, and there is an overlapping player,
	 * the method logs a message indicating that the player is teleporting and calls the PrepForTeleport method.
	 *
	 * @param DeltaTime The time in seconds since the last frame.
	 */
	virtual void Tick(float DeltaTime) override;
	
private:
	/**
	 * @brief TeleportDelayTimer is a float variable representing the delay time for teleportation.
	 *
	 * This variable is editable from anywhere and has a default value of 1 second.
	 * It can be adjusted to change the delay time for teleportation.
	 *
	 * Example Usage:
	 *
	 * TeleportDelayTimer = 2.5f; // Set the delay time to 2.5 seconds
	 *
	 * @see Teleport()
	 */
	UPROPERTY(EditAnywhere)
	float TeleportDelayTimer = 1.f;

	/**
	 * @brief Teleport source door variable.
	 *
	 * This variable holds a reference to a teleport source door.
	 * It is an Unreal Engine property that can be edited in the editor.
	 * The type of the variable is ADoor*, which represents a door object in the game.
	 *
	 * @see ADoor
	 */
	UPROPERTY(EditAnywhere)
	ADoor* TeleportSourceDoor;

	/**
	 * @brief GlobalTeleportTargetLocation is a variable used to store the target location for a global teleport.
	 *
	 * This variable is of type FVector and is editable anywhere. It is used to store the destination location for a
	 * global teleportation event. The location can be set and modified from any blueprint or code that has access to
	 * this variable.
	 */
	UPROPERTY(EditAnywhere)
	FVector GlobalTeleportTargetLocation;

	/**
	 * @brief Indicates whether the player character is currently overlapping the trigger.
	 *
	 * This variable is a boolean that is set to true when the player character overlaps the trigger
	 * and set to false when the player character stops overlapping the trigger.
	 */
	UPROPERTY(VisibleAnywhere)
	bool bPlayerIsOverlappingTrigger;

	/**
	 * A Blueprint-visible reference to the overlapping player.
	 *
	 * This variable holds a pointer to an instance of the APC_PlayerFox class, representing the player character
	 * currently overlapping this actor. The pointer is read-only and can be accessed from Blueprint scripts
	 * and C++ code.
	 *
	 * @see APC_PlayerFox
	 */
	UPROPERTY(VisibleAnywhere)
	const APC_PlayerFox* OverlappingPlayer;

	/**
	 * @brief The handle for the teleport delay timer.
	 *
	 * This variable stores the handle for the teleport delay timer.
	 * It is used to manage the timer and perform actions when the timer completes.
	 *
	 * @note The timer handle is of type FTimerHandle, which is a struct provided by the Unreal Engine.
	 *
	 * @see FTimerHandle
	 */
	FTimerHandle TeleportDelayTimerHandle;

	/**
	 * @brief Represents a timer delegate for teleport delay.
	 *
	 * This variable is used to specify a timer delegate that will be triggered after a certain delay
	 * during the teleportation process.
	 *
	 * @note The timer delegate should be bound to a function or a lambda expression that will be executed
	 *       when the timer is triggered.
	 */
	FTimerDelegate TeleportDelayDelegate;

	/**
	 * @brief TeleportSound variable.
	 *
	 * This variable represents the sound to be played during teleportation.
	 */
	UPROPERTY(EditAnywhere)
	USoundBase* TeleportSound;

	/**
	 * Plays the teleport sound for the given player.
	 *
	 * @param Player The player to play the sound for.
	 */
	UFUNCTION(BlueprintCallable)
	void PlayTeleportSound(const APC_PlayerFox* Player);

	/**
	 * @brief Prepares the player for teleportation by setting up a delay timer to trigger the teleport.
	 *
	 * This method is BlueprintCallable, allowing it to be called from Blueprint scripts.
	 *
	 * @param Player A pointer to the APC_PlayerFox object to teleport.
	 *
	 * @note The TeleportDelayDelegate is bound to the Teleport method with the given player as a parameter.
	 *       The TeleportDelayDelegate will be triggered after the specified TeleportDelayTimer duration, causing
	 *       the teleport to be executed. The TeleportDelayDelegate is bound using the BindUFunction method,
	 *       which binds a member function to a delegate and retains the object upon calling it. The
	 *       TeleportDelayTimerHandle is set to the handle returned by the SetTimer method, which registers
	 *       the TeleportDelayDelegate to be called after the specified delay. The TeleportDelayTimerHandle is
	 *       used to manage the timer for later cancelling if needed.
	 *
	 * @see Teleport
	 * @see TeleportDelayDelegate
	 * @see TeleportDelayTimerHandle
	 */
	UFUNCTION(BlueprintCallable)
	void PrepForTeleport(const APC_PlayerFox* Player);

protected:
	/**
	 * NotifyActorBeginOverlap method is called when an actor begins overlapping with this actor.
	 *
	 * @param OtherActor The actor that has begun overlapping with this actor.
	 */
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	/**
	 * Notifies when an actor ends overlap with this trigger.
	 *
	 * @param OtherActor The actor that has ended overlap with this trigger.
	 */
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
};
