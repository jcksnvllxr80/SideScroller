// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SideScrollerGameState.h"
#include "LobbyGameState.generated.h"

/**
 * @class ALobbyGameState
 * @brief A class representing the lobby game state in a side scroller game.
 *
 * The ALobbyGameState class is derived from the ASideScrollerGameState class and extends its functionality
 * to include player character selection and opening the character selection menu.
 */
UCLASS()
class SIDESCROLLER_API ALobbyGameState : public ASideScrollerGameState
{
	GENERATED_BODY()

public:
	/**
	 * @brief Spawns the character select window for the player.
	 *
	 * This method is called when the player spawns in the game. It checks if the game instance is valid,
	 * gets the player controller, and checks if the player has already chosen a character. If the player
	 * hasn't chosen a character yet, it opens the character select window. Otherwise, it logs a warning message.
	 *
	 * @param None
	 *
	 * @return None
	 */
	void PlayerSpawnCharacterSelect();

	/**
	 * Open the select character menu.
	 *
	 * This method checks if the LobbyGameState and GameInstance are valid,
	 * and if so, it calls the SelectCharacterLoadMenu method of the GameInstance.
	 * If the LobbyGameState or GameInstance are not valid, it logs a warning message.
	 *
	 * @param None
	 * @return None
	 */
	UFUNCTION(BlueprintCallable)
	void OpenSelectCharacterMenu();
	
private:
	/**
	 * @brief Timer handle for character select delay.
	 *
	 * This variable represents the timer handle used for character select delay functionality.
	 * It is of type FTimerHandle and is used to manage timers for delaying character selection.
	 * The timer handle can be used to start, stop, and manage the execution of a specific timer.
	 *
	 * @see FTimerHandle
	 */
	FTimerHandle CharacterSelectDelayTimerHandle;

	/**
	 * @brief The delay timer for character selection.
	 *
	 * This variable represents the delay timer for character selection. It is used to determine the time interval
	 * between character selections. The value of this variable is editable by the editor, allowing users to
	 * customize the delay timer according to their preferences.
	 *
	 * @note The default value of this variable is 0.25.
	 */
	UPROPERTY(EditAnywhere)
	float CharacterSelectDelayTimer = 0.25;

protected:
	/**
	 * \brief This method is called when the game begins play.
	 *
	 * This virtual method is called when the game starts or when the designated play-in editor is launched. It can
	 * be overridden by subclasses to implement custom behavior.
	 *
	 * When this method is called, it calls the base implementation using the Super keyword and then sets a timer
	 * using the TimerManager. The timer is set to call the PlayerSpawnCharacter Select method after a certain delay.
	 *
	 * \see PlayerSpawnCharacterSelect
	 */
	virtual void BeginPlay() override;
};
