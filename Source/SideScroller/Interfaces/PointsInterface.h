// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PointsInterface.generated.h"

/**
 *
 */
UINTERFACE(MinimalAPI)
class UPointsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * @interface IPointsInterface
 * @brief Interface for giving points to a player character.
 *
 * This interface provides a function to give points to a player character.
 */
class SIDESCROLLER_API IPointsInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * @brief Gives points to a player character.
	 *
	 * This method is used to give points to a player character in the game.
	 * The points are given based on certain conditions or events in the game.
	 *
	 * @param PlayerChar The player character to give points to.
	 *
	 * @note This method is a pure virtual function and must be implemented
	 *       by inheriting classes.
	 *
	 * @see UFUNCTION
	 */
	UFUNCTION(Category="Points")
	virtual void GivePoints(APC_PlayerFox* PlayerChar) = 0;
};
