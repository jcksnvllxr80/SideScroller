// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "GameOverMenu.generated.h"

/**
 * @class UGameOverMenu
 * @brief Represents a game over menu widget.
 *
 * This class inherits from UMenuWidget and provides functionality for a game over menu.
 */
UCLASS()
class SIDESCROLLER_API UGameOverMenu : public UMenuWidget
{
	GENERATED_BODY()

protected:
	/**
	 * Initializes the GameOverMenu.
	 *
	 * @return True if the initialization was successful, false otherwise.
	 */
	UFUNCTION()

	virtual bool Initialize() override;
	
private:
	/**
	 * @brief RestartGameButton Description
	 *
	 * This variable represents a button widget used for restarting the game.
	 * It is a UButton instance that is bound with a widget in the user interface (UI).
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* RestartGameButton;

	/**
	 * @brief The reference to the main menu button widget.
	 *
	 * This variable represents the reference to the main menu button widget
	 * in the blueprint or C++ code. It is decorated with the UPROPERTY macro and
	 * the meta attribute "BindWidget" to enable binding with the corresponding UI
	 * element in the user interface editor or blueprint.
	 *
	 * Clients can use this variable to interact with the main menu button widget, such as
	 * adding event handlers, changing visibility, or updating properties.
	 *
	 * @see UButton
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* MainMenuButton;

	/**
	 * @brief QuitButton variable.
	 *
	 * This variable represents the QuitButton widget in the UI. It is of type UButton*.
	 * The QuitButton widget is used to allow the user to quit or exit the current application.
	 * It is bound to a button widget using the BindWidget meta attribute.
	 *
	 * @see UButton
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	/**
	 * @brief Function called when the user selects the option to go back to the main menu.
	 *
	 * This function is called when the user selects the option to go back to the main menu from the
	 * game over menu. It checks if the MenuInterface is valid and then calls the LoadMainMenu function
	 * on the MenuInterface. If the MenuInterface is not valid, an error message is logged.
	 *
	 * @param None
	 *
	 * @return None
	 *
	 * @note This function assumes that the MenuInterface has been set correctly before calling this function.
	 */
	UFUNCTION()
	void BackToMainMenu();

	/**
	 * Restarts the game.
	 *
	 * This method will end the current game session and start a new game session with the specified level. It is
	 * recommended to call this method from a game over menu or similar UI element.
	 *
	 * @param None
	 *
	 * @return None
	 */
	UFUNCTION()
	void RestartGame();

	/**
	 * Quits the game by sending the quit command to the player controller's console.
	 *
	 * @param None
	 * @return None
	 */
	UFUNCTION()
	void QuitGame();
};
