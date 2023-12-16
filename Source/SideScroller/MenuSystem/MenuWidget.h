// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Sidescroller/Interfaces/MenuInterface.h"
#include "MenuWidget.generated.h"


/**
 * @class UMenuWidget
 * @brief A user widget class used for menu screens.
 *
 * This class is derived from UUserWidget and provides functionality to handle menu screens.
 */
UCLASS()
class SIDESCROLLER_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * Set the menu interface for the menu widget.
	 *
	 * @param MenuInterface The menu interface to be set.
	 */
	void SetMenuInterface(IMenuInterface* MenuInterface);
	/**
	 * @brief Sets up the menu widget and adds it to the viewport.
	 *
	 * This method adds the menu widget to the viewport and sets up the input mode for the player controller.
	 * It also shows the mouse cursor.
	 */
	void Setup();
	/**
	 * Called when the widget's associated level is removed from the world.
	 *
	 * This method is overridden from the base UObject class. It is called by the engine when the widget's
	 * associated level is removed from the world. The method performs necessary clean-up tasks and detaches
	 * the widget from its parent.
	 *
	 * @param None
	 * @return None
	 */
	virtual void OnLevelRemovedFromWorld();

protected:
	/**
	 * @class MenuInterface
	 * @brief An interface for handling menu functionality.
	 *
	 * This interface defines the methods that need to be implemented by any class that wants to provide
	 * menu functionality.
	 */
	IMenuInterface* MenuInterface;
};
