// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "RespawnMenu.generated.h"

/**
 * @class URespawnMenu
 * @brief Represents the respawn menu widget in a side-scroller game.
 * Inherits from UMenuWidget.
 */
UCLASS()
class SIDESCROLLER_API URespawnMenu : public UMenuWidget
{
	GENERATED_BODY()
	
protected:
	/**
	 * Runs the respawn timer.
	 *
	 * This method is responsible for showing the respawn countdown on the respawn menu and setting a timer
	 * to enable the respawn button after a specified delay.
	 *
	 * @param None
	 *
	 * @return None
	 *
	 * @see URespawnMenu::EnableRespawnButton()
	 */
	UFUNCTION()
	void RunRespawnTimer();

	/**
	 * Initialize the Respawn Menu.
	 *
	 * @return True if initialization is successful, false otherwise.
	 */
	UFUNCTION()
	virtual bool Initialize() override;

private:
	/**
	 * @class RespawnButton
	 * @brief Represents a button used for respawning in a game.
	 *
	 * The RespawnButton class is a UPROPERTY that is bound to a UButton widget in Unreal Engine.
	 *
	 * Example usage:
	 * @code
	 *     UPROPERTY(meta = (BindWidget))
	 *     class UButton* RespawnButton;
	 * @endcode
	 *
	 * For more information on how to use this class, refer to the Unreal Engine documentation.
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* RespawnButton;

	/**
	 * @brief Represents a main menu button widget in the UI.
	 *
	 * The MainMenuButton variable is a UButton* pointer. It is annotated
	 * with the UPROPERTY macro with the meta tag (BindWidget), which helps
	 * in binding the UButton* pointer to the associated widget in Blueprint.
	 *
	 * Usage example:
	 *
	 * UPROPERTY(meta = (BindWidget))
	 *     class UButton* MainMenuButton;
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* MainMenuButton;

	/**
	 * @brief QuitButton is a variable that represents a button widget for quitting the application.
	 *
	 * The QuitButton variable is a UButton object that is bound to a button widget in the user interface.
	 * It is used to enable the user to quit the application when the button is clicked.
	 *
	 * Usage:
	 * - Bind a button widget in the user interface to the QuitButton variable using UPROPERTY(meta = (BindWidget)).
	 * - Set the OnClicked event of the QuitButton to execute the quit logic.
	 *
	 * Example:
	 * @code
	 * UPROPERTY(meta = (BindWidget))
	 *     class UButton* QuitButton;
	 * @endcode
	 */
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	/**
	 * @brief The handle for the respawn delay timer.
	 *
	 * This variable is used to hold the reference to the respawn delay timer. It is of type FTimerHandle,
	 * which is a struct that works as a handle to manage a timer.
	 *
	 * The respawn delay timer is typically used in game development to specify a delay before a character
	 * or game object is respawned after death or destruction.
	 */
	FTimerHandle RespawnDelayTimerHandle;

	/**
	 * @brief The amount of time it takes for an object to respawn after being deactivated.
	 *
	 * @details This variable specifies the delay time in seconds for an object to respawn after it has
	 * been deactivated.
	 *
	 * @note The default value is 2 seconds.
	 */
	UPROPERTY()
	float RespawnDelayTime = 2.f;

	/**
	 * \brief Respawn the player at the last checkpoint.
	 *
	 * This method is used to respawn the player at the last checkpoint. It checks if the world and the player
	 * controller exist, and then checks if the player's pawn exists. If the player's pawn is valid, it logs a
	 * message indicating that the player is respawning and calls the ReviveAtCheckpoint method on the player's pawn.
	 *
	 * \param None
	 *
	 * \return None
	 */
	UFUNCTION()
	void Respawn();

	/**
	 * @brief This method is used to go back to the main menu.
	 *
	 * This method is called when the user wants to return to the main menu
	 * from the respawn menu. It checks if the MenuInterface exists, and if it does,
	 * it calls the LoadMainMenu() method on it. If the MenuInterface does not exist,
	 * an error message is logged and the method returns.
	 *
	 * @param None.
	 * @return None.
	 */
	UFUNCTION()
	void BackToMainMenu();

	/**
	 * Quit the game.
	 *
	 * This method quits the game by executing the 'quit' console command.
	 * If the player controller or the world cannot be found, an error message will be logged
	 * and the method will return.
	 *
	 * @param None
	 * @return None
	 */
	UFUNCTION()
	void QuitGame();

	/**
	 * @brief Enable the respawn button.
	 *
	 * This method enables the respawn button in the RespawnMenu.
	 *
	 * @param None.
	 *
	 * @return None.
	 */
	UFUNCTION()
	void EnableRespawnButton() const;

	/**
	 * @brief Function to return back to the game.
	 *
	 * This function removes the level from the world, resumes the game, and allows the player to continue playing.
	 *
	 * @param None
	 * @return None
	 *
	 * @note This function is marked as a UFUNCTION to collaborate with Unreal's reflection system.
	 * @warning This function assumes that there is a valid world and player controller present.
	 */
	UFUNCTION()
	void BackToGame();
};
