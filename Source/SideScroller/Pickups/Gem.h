// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickup.h"
#include "SideScroller/Interfaces/PickupInterface.h"
#include "Gem.generated.h"

/**
 * @class AGem
 * @brief Represents a gem pickup in a side-scrolling game.
 *
 * This class inherits from ABasePickup and IPickupInterface. It provides functionality to give the pickup
 * to the player and updates its behavior over time.
 */
UCLASS()
class SIDESCROLLER_API AGem : public ABasePickup, public IPickupInterface
{
	GENERATED_BODY()

public:
	/**
	 * @brief This method is called every frame to update the state of the AGem object.
	 *
	 * @param DeltaTime The time in seconds since the last frame.
	 *
	 * This method is a virtual override of the Tick method in the base class. It is called every frame
	 * to update the state of the AGem object. This method does not return a value.
	 *
	 * The AGem object's state is updated by calling the base class's Tick method with the DeltaTime parameter.
	 * The base class's Tick method performs basic tick functionality, such as updating the actor's components
	 * and calling the Tick functions of those components.
	 *
	 * @see Super::Tick
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * Gives the pickup to the specified player character.
	 *
	 * @param OverlappingActor The player character to give the pickup to.
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	virtual void GivePickup(APC_PlayerFox* OverlappingActor) override;

	/**
	 * @brief Default constructor for AGem class.
	 *
	 * Sets up the AGem actor with proper configuration.
	 *
	 * @param None
	 * @return None
	 */
	AGem();

protected:
	/**
	 * @brief This method is called when the game begins.
	 *
	 * This method is called when the game begins and should be overridden
	 * in child classes to perform any initialization or setup needed for
	 * the game to start running.
	 *
	 * @param None
	 *
	 * @return None
	 */
	virtual void BeginPlay() override;

private:
	/**
	 * @brief MonetaryValue represents the amount of money.
	 *
	 * This variable is an integer that stores the monetary value.
	 * It is used to indicate the amount of money associated with a certain entity.
	 * The value of MonetaryValue can be modified through external dependencies.
	 *
	 */
	UPROPERTY(EditAnywhere)
	int MonetaryValue = 5;
};
