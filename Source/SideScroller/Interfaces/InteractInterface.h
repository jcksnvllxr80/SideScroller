// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

/**
 * \class ABasePaperCharacter
 *
 * \brief The base class for all paper characters in the game.
 *
 * The ABasePaperCharacter class represents the base class for all characters in the game that use
 * paper flipbook animations. It provides functionality for handling animations, shooting projectiles,
 * taking damage, and performing death actions.
 *
 * \note This class is derived from the APaperCharacter class provided by the Unreal Engine.
 */
class ABasePaperCharacter;
/**
 * @class UInteractInterface
 *
 * This interface provides the basic functionality for objects that can be interacted with.
 */
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * @class IInteractInterface
 *
 * @brief The interface for interacting with objects.
 *
 * This interface defines the method that needs to be implemented by any class
 * that wants to allow interaction with it. This allows different classes to have
 * different types of interactions.
 */
class SIDESCROLLER_API IInteractInterface
{
	GENERATED_BODY()

public:
	/**
	 * Interacts with an object.
	 *
	 * This method is used to interact with an object in the system.
	 *
	 * @param No parameters are passed to this method.
	 *
	 * @return This method does not return any value.
	 *
	 * @category Interact
	 */
	UFUNCTION(Category="Interact")
	virtual void Interact() = 0;
};
