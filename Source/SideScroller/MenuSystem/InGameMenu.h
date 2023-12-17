// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "InGameMenu.generated.h"

/**
 * @class UInGameMenu
 * @brief Represents an in-game menu widget class
 *
 * This class extends UMenuWidget and provides functionality for an in-game menu.
 */
UCLASS()
class SIDESCROLLER_API UInGameMenu : public UMenuWidget
{
	GENERATED_BODY()

protected:
	/**
	 * Initializes the in-game menu.
	 *
	 * @return True if the initialization is successful, false otherwise.
	 */
	virtual bool Initialize() override;
	
private:
	/**
	 * @brief This variable represents the Cancel Button widget.
	 *
	 * The CancelButton variable is a UButton pointer that is used to refer to the Cancel Button widget.
	 * It is marked with the UPROPERTY macro to indicate that it is a bound widget that can be accessed from Blueprint.
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;

	/**
	 * @brief UButton variable representing the exit button in the UI.
	 *
	 * This variable is marked with the `UPROPERTY` macro and is used to bind the
	 * exit button widget in the UI to the variable in the code. It is an instance
	 * of the `UButton` class.
	 *
	 * @see UButton
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	/** \brief BackToMainMenu method.
	 
	 The BackToMainMenu method is used to transition the player back to the main menu.
	 
	 \param None
	 
	 \return None
	 
	 \note This method should only be called when the player wants to go back to the main menu.
	 
	 */
	UFUNCTION()
	void BackToMainMenu();

	/**
	 * @brief Resumes the game from the in-game menu.
	 *
	 * This method is called when the player selects the "Back to Game" option from the in-game menu.
	 * It resumes the game by setting the pause state of the player controller to false.
	 *
	 * The method also calls the OnLevelRemovedFromWorld() function which is responsible for any necessary cleanup
	 * or level-specific operations before the level is removed from the world.
	 *
	 * @note This method should be called by the UI system when the "Back to Game" button is pressed.
	 */
	UFUNCTION()
	void BackToGame();
	
};
