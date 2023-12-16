// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "MovingPlatform.generated.h"

/**
 * @class AMovingPlatform
 *
 * @brief Represents a moving platform in a side-scrolling game.
 *
 * This class is derived from APaperSpriteActor and provides functionality to move a platform from its starting location
 * to a target location with a certain speed. The platform can be triggered by activating or deactivating a trigger.
 */
UCLASS()
class SIDESCROLLER_API AMovingPlatform : public APaperSpriteActor
{
	GENERATED_BODY()
	
public:
	/**
	 * \brief Constructor for the AMovingPlatform class.
	 *
	 * This constructor initializes the AMovingPlatform object by setting up some default properties.
	 */
	AMovingPlatform();

	/**
	 * \brief Called when the game starts or when spawned.
	 *
	 * This method is called by the engine when the game starts or when the actor is spawned.
	 * It initializes the necessary properties and settings for the moving platform.
	 */
	virtual void BeginPlay() override;
	/** Executes tick operations for the Moving Platform.
	 *  @param DeltaTime The time between the last frame and the current frame.
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief The Speed variable represents the speed value.
	 *
	 * The Speed variable is a floating-point number that represents the speed value. It is editable anywhere
	 * and has a default value of 100. This variable can be used to control the movement speed of an object
	 * or to adjust the rate at which certain actions are performed.
	 *
	 * @note The value assigned to Speed should be in units appropriate for the specific use case.
	 *
	 * @see Other related variables: Acceleration, MaxSpeed
	 */
	UPROPERTY(EditAnywhere)
	float Speed = 100;

	/**
	 * @brief The target location that the user wants to move an object to.
	 *
	 * The target location is a FVector that represents the desired position in the game world where the user wants to
	 * move an object to. The variable is annotated with UPROPERTY to make it editable in the Unreal Engine editor.
	 * The Meta tag MakeEditWidget=true allows the user to visually select a location in the editor, improving the
	 * usability of the variable.
	 *
	 * @see FVector
	 */
	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	FVector TargetLocation;

	/**
	 * @brief Increments the counter of active triggers.
	 *
	 * This method is used to increment the counter of active triggers in the AMovingPlatform class.
	 * It is typically called when a trigger becomes active.
	 */
	void AddActiveTrigger();
	/**
	 * @brief Remove an active trigger from the moving platform.
	 *
	 * This method is used to remove an active trigger from the moving platform.
	 * It decreases the count of active triggers by one.
	 *
	 * @note The method does nothing if there are no active triggers.
	 *
	 * @see SetActiveTriggers()
	 */
	void RemoveActiveTrigger();

private:
	/**
	 * @brief GlobalTargetLocation
	 *
	 * Stores the global target location in the 3D world.
	 *
	 * The GlobalTargetLocation variable is of type FVector and is used to store the global target
	 * location in the 3D world. This variable can be accessed and modified by various functions and
	 * classes within the project.
	 *
	 * The FVector class represents a vector in 3D space with X, Y, and Z coordinates.
	 *
	 * Usage:
	 *   - To access the current global target location, use GlobalTargetLocation.
	 *   - To set a new global target location, assign a new FVector value to GlobalTargetLocation.
	 *   - Example usage: GlobalTargetLocation = FVector(10.0f, 0.0f, 5.0f);
	 *
	 * Note:
	 *   - It is important to update the GlobalTargetLocation variable whenever the target location
	 *     changes in the game.
	 */
	FVector GlobalTargetLocation;
	/**
	 * @brief The location in 3D space where the global start is set.
	 *
	 * This variable represents a position in the 3D space using an FVector object. It denotes the starting location
	 * for a global operation or process. The position is specified using three coordinates (X, Y, and Z)
	 * within a Cartesian coordinate system. This global start location is used as a reference point for various
	 * calculations or operations.
	 *
	 * @see FVector
	 */
	FVector GlobalStartLocation;

	/**
	 * @brief The number of active triggers.
	 *
	 * This variable represents the count of active triggers in the system. It is an integer value that can be
	 * modified and replicated in the network. The default value is 1.
	 *
	 * @note This variable can be edited anywhere and its value can be synchronized across the network.
	 */
	UPROPERTY(EditAnywhere, replicated)
	int ActiveTriggers = 1;
};
