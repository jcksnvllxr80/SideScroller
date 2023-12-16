// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PC_AIController.generated.h"

/**
 * \class APC_AIController
 * \brief A class that represents the AI controller for the AI-controlled characters in the game.
 *
 * This class inherits from AAIController, which provides basic AI behavior.
 * It controls the movement and actions of AI characters in the game.
 */
UCLASS()
class SIDESCROLLER_API APC_AIController : public AAIController
{
	GENERATED_BODY()

public:
	/**
	 * Constructor for the APC_AIController class.
	 */
	APC_AIController();

	/**
	 * @brief Destroys the actor.
	 *
	 * This function should be called to destroy the actor associated with the AI controller.
	 * It clears any ongoing shoot timer intervals associated with the actor.
	 *
	 * @param None.
	 *
	 * @return None.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void DestroyActor();

	/**
	 * @brief Sets the can shoot status to true.
	 *
	 * This method sets the can shoot status of the AI controller to true,
	 * indicating that the AI can shoot again.
	 *
	 * @param None.
	 * @return None.
	 */
	void CanShootAgain();

	/**
	 * @brief Ticks the controller.
	 *
	 * This method is called every frame to update the controller's behavior.
	 *
	 * @param DeltaSeconds The time passed in seconds since the last frame.
	 *
	 * @return None.
	 */
	virtual void Tick(float DeltaSeconds) override;	
	
private:
	/**
	 * @var PlayerPawn
	 *
	 * @brief A pointer to the controlled pawn of the player.
	 *
	 * @details This variable is used to store the reference to the pawn that is being controlled by the player.
	 * It is of type APawn* and is initialized to nullptr by default.
	 *
	 * @note In order to prevent dangling pointer issues, make sure to set this variable to nullptr when the
	 * pawn is destroyed.
	 *
	 * @code
	 * PlayerPawn = nullptr;
	 * @endcode
	 *
	 * @see APawn
	 */
	APawn* PlayerPawn = nullptr;

	/**
	 * @brief Handle for the shoot timer.
	 *
	 * This variable serves as a handle for the shoot timer. It can be used to start, stop, or modify the timer.
	 */
	FTimerHandle ShootTimerHandle;

	/**
	 * @brief Specifies whether shooting is allowed or not.
	 *
	 * This variable represents the current state of whether shooting is allowed or not.
	 * If the value is true, it means shooting is allowed.
	 * If the value is false, it means shooting is not allowed.
	 */
	bool CanShoot = true;

protected:
	/**
	 * \brief Called when the game starts or when spawned.
	 * \details This is a virtual method that is called when the game starts or when the actor is spawned in the world.
	 *          It is used to perform initialization tasks or setting up references to other actors or components.
	 * \attention This method should be overridden in child classes to provide custom implementation.
	 * \note In this particular implementation, it calls the parent's BeginPlay() method and then sets up a
	 * reference to the player pawn.
	 */
	virtual void BeginPlay() override;

	/**
	 * UpdateFocusPawn method updates the focus pawn for the AI controller.
	 *
	 * @return boolean - true if the focus pawn is successfully updated, otherwise false.
	 */
	UFUNCTION(BlueprintCallable)
	bool UpdateFocusPawn();

	/**
	 * Focuses on the pawn and performs shooting action if conditions are met.
	 */
	UFUNCTION(BlueprintCallable)
	void FocusOnPawn();
};
