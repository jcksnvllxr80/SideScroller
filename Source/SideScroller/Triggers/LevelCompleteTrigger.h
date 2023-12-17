// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"
#include "SideScroller/Controllers/GameModePlayerController.h"
#include "LevelCompleteTrigger.generated.h"

/**
 * @class ALevelCompleteTrigger
 *
 * @brief A class representing a trigger box that detects when a level is complete.
 *
 * This class inherits from ATriggerBox and adds additional functionality to handle level completion.
 */
UCLASS()
class SIDESCROLLER_API ALevelCompleteTrigger : public ATriggerBox
{
	GENERATED_BODY()

public:
	/**
	 * \brief Notifies when an actor begins to overlap this actor.
	 *
	 * This method is called when an actor starts to overlap the level complete trigger actor.
	 * It performs the following actions:
	 * 1. Calls the base class implementation of NotifyActorBeginOverlap to handle any additional functionality.
	 * 2. Attempts to cast the overlapping actor to the PC_PlayerFox class.
	 * 3. If the cast is successful, logs a message indicating that the PC_PlayerFox actor is overlapping the
	 * level complete trigger actor. It also displays the name of the PC_PlayerFox
	 * actor.
	 * 4. Calls the PrepForNextLevel method to prepare for the next level with the PC_PlayerFox actor
	 * as the parameter.
	 *
	 * \param OtherActor The actor that is overlapping the level complete trigger.
	 */
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	/**
	 * @brief Called when the game starts or when spawned.
	 *
	 * This function is called when the game starts or when the actor is spawned. It is a virtual function
	 * that can be overridden in derived classes to add custom initialization logic.
	 */
	virtual void BeginPlay() override;

private:
	/**
	 * @brief Prepares the player for the next level.
	 *
	 * This method is called to prepare the player for the next level. It displays the level complete
	 * banner/celebration and sets up a timer to start the next level after a specified delay.
	 *
	 * @param Player The player character that needs to be prepared for the next level.
	 *
	 * @note This method relies on the player character having a valid controller of type
	 * AGameModePlayerController. If the player's controller is not of the expected type, a warning will
	 * be logged and no further action will be taken.
	 */
	UFUNCTION()
	void PrepForNextLevel(APC_PlayerFox* Player);

	/**
	 * \brief Calls the StartNextLevel method on the specified GameModePlayerController.
	 *
	 * \param GameModePlayerController The GameModePlayerController on which the StartNextLevel method will be called.
	 *
	 * This method is used to call the StartNextLevel method on the specified GameModePlayerController.
	 * It is a static method and does not have a return value.
	 *
	 * Example Usage:
	 * \code{.cpp}
	 * AGameModePlayerController* PlayerController = GetPlayerController();
	 * CallNextLevelStart(PlayerController);
	 * \endcode
	 *
	 * \see GameModePlayerController
	 */
	UFUNCTION()
	static void CallNextLevelStart(AGameModePlayerController* GameModePlayerController);

	/** \var DoorSize
	 *  \brief The size of the door.
	 *
	 *  This variable represents the size of the door in 3D space. It is a vector with
	 *  three components: width, height, and depth.
	 *
	 *  The default value of DoorSize is (8.0, 20.0, 16.0), which corresponds to a standard door size.
	 *  The values are in centimeters.
	 *
	 *  This variable can be edited in the editor, allowing for customization of the door size in each instance.
	 *
	 *  \sa DoorComponent
	 */
	UPROPERTY(EditAnywhere)
	FVector DoorSize = {8.000000, 20.000000, 16.000000};

	/**
	 * @brief The StartNextLevelDelayTimer variable is a float that represents the delay (in seconds) before
	 * starting the next level.
	 *
	 * @details This variable is an EditAnywhere UPROPERTY, which means it can be edited in the Unreal Engine editor.
	 * The default value for this variable is 2.f seconds.
	 * The value of this variable determines the delay before starting the next level after the current level is
	 * completed.
	 *
	 * @note This variable should be set in the editor to specify the desired delay before starting the next level.
	 *
	 * @see EditAnywhere
	 */
	UPROPERTY(EditAnywhere)
	float StartNextLevelDelayTimer = 2.f;

	/**
	 * @brief Class: StartNextLevelDelayTimerHandle
	 *
	 * @details The StartNextLevelDelayTimerHandle is a handle that allows for manipulation and tracking of a
	 * timer delay for starting the next level.
	 *
	 * This variable is of type FTimerHandle, which is a structure provided by Unreal Engine. It is used to
	 * store a handle to a timer, allowing for the ability to start, stop, or modify
	 * the timer as needed. This specific timer handle is used to track the delay before starting the next level
	 * in a game or application.
	 */
	FTimerHandle StartNextLevelDelayTimerHandle;

	/**
	 * @brief The delegate type for the delayed start of the next level.
	 *
	 * This delegate is used to store a function pointer along with an object pointer,
	 * and can be used to execute the function after a certain delay.
	 */
	FTimerDelegate StartNextLevelDelayDelegate;
};
