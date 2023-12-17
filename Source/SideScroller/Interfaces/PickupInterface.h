// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickupInterface.generated.h"

/**
 * \class ABasePaperCharacter
 *
 * \brief The ABasePaperCharacter class is a base class for paper characters in a side-scrolling game.
 *
 * The ABasePaperCharacter class inherits from the APaperCharacter class, which is a built-in Unreal Engine class
 * for characters using 2D sprites. This class provides functionality for idle, death, and hurt animations,
 * shooting projectiles, taking damage, and performing death actions.
 *
 * @note This class is part of the SIDESCROLLER_API plugin and can be used by derived classes.
 */
class ABasePaperCharacter;
/**
 * @brief Interface for objects that can be picked up in the game.
 */
UINTERFACE(MinimalAPI)
class UPickupInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * \class IPickupInterface
 * \brief An interface for implementing pickups.
 *
 * This interface provides a method for implementing pickups in a side-scrolling game.
 */
class SIDESCROLLER_API IPickupInterface
{
	GENERATED_BODY()

public:
	/**
	 * @brief GivePickup method gives a pickup item to the specified player.
	 *
	 * This method is used to give a pickup item to the player specified by the `OverlappingActor` parameter.
	 * The pickup item can be any type of item or power-up that the player can collect during gameplay.
	 *
	 * @param OverlappingActor The player actor that is overlapping or colliding with the pickup item.
	 *
	 * @see APC_PlayerFox
	 */
	UFUNCTION(Category="Pickup")
	virtual void GivePickup(APC_PlayerFox* OverlappingActor) = 0;
};
