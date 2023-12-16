// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SideScrollerGameModeBase.h"
#include "LevelGameMode.generated.h"

/**
 * @class ALevelGameMode
 * @brief A class representing the game mode for a level in a side-scroller game.
 *
 * This class is responsible for managing the game logic and mechanics for a specific level.
 */
UCLASS()
class SIDESCROLLER_API ALevelGameMode : public ASideScrollerGameModeBase
{
	GENERATED_BODY()

public:
	/**
	 * @brief The default character blueprint variable.
	 *
	 * This variable represents the blueprint class of the default character that will be used in the game.
	 * It is an editable property and can be accessed and modified in the blueprint editor.
	 * By default, it is set to nullptr.
	 *
	 * @see APC_PlayerFox
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Classes)
	TSubclassOf<APC_PlayerFox> DefaultCharacterBP = nullptr;

	/**
	 * Starts the next level in the game.
	 *
	 * This method is used to start the next level in the game. It first retrieves the game instance
	 * and checks if it is valid. If the game instance is null, a warning message is logged and the
	 * method returns. Otherwise, a log message is displayed indicating the completion of the
	 * current level. The current level is then incremented in the game instance.
	 *
	 * If the game world is not available, the method returns. Otherwise, seamless travel is enabled
	 * and a travel URL is generated for the next level map. The method then calls the ServerTravel
	 * method of the game world with the generated travel URL, which initiates the transition to the
	 * next level.
	 *
	 * @param None.
	 * @return None.
	 */
	UFUNCTION(BlueprintCallable)
	void StartNextLevel();

private:
	/**
	 * Locates the chosen character for the given player controller and spawns it in the game world.
	 *
	 * @param Iter The iterator for the player controller.
	 * @return True if the chosen character is successfully spawned, false otherwise.
	 */
	bool LocateChosenCharacter(FConstPlayerControllerIterator Iter);
	/**
	 * Spawns the player chosen characters.
	 *
	 * This method iterates through all player controllers in the world and attempts to locate and spawn each player's
	 * chosen character. If the world is not found, an error message is logged and the method returns.
	 *
	 * @param None
	 * @return None
	 */
	void SpawnPlayerChosenCharacters();

	/**
	 * @brief The amount of delay before spawning the player's chosen character.
	 *
	 * @details During gameplay, when the player chooses a character to spawn, there
	 *          may be a small delay before the character actually spawns. This delay
	 *          can be customized using the `SpawnPlayerChosenCharDelayTimer` variable.
	 *          It is editable in the Unreal Engine Editor and can be adjusted as desired.
	 *
	 * @note The default value for this variable is 0.3 seconds.
	 *
	 * @see PlayerCharacter
	 */
	UPROPERTY(EditAnywhere)
	float SpawnPlayerChosenCharDelayTimer = 0.3;

	/**
	 * @brief Timer handle for delaying the spawn of player chosen character.
	 *
	 * This variable is used to control the delay in spawning the player's chosen character. It is of type FTimerHandle,
	 * which represents a handle to a timer delegate. By using this handle, you can start, stop, or reset the
	 * timer associated with it.
	 *
	 * Usage:
	 * - To start the timer, call the StartTimer function passing in this handle.
	 * - To stop the timer, call the ClearTimer function passing in this handle.
	 * - To reset the timer, call the ResetTimer function passing in this handle.
	 *
	 * @see FTimerHandle
	 */
	FTimerHandle SpawnPlayerChosenCharDelayTimerHandle;

protected:
	/**
	 * @brief Called when the level begins playing.
	 *
	 * This method is called when the level begins playing. It is a virtual method
	 * that can be overridden by derived classes to perform additional functionality
	 * specific to the game mode.
	 *
	 * @details The BeginPlay method is responsible for setting up any necessary
	 * timers and initializing the level. It calls the Super::BeginPlay method to
	 * perform any necessary setup in the parent class.
	 *
	 * The SpawnPlayerChosenCharacters method is scheduled to be called after a
	 * delay, as specified by the SpawnPlayerChosenCharDelayTimer parameter. Once
	 * the delay has passed, the SpawnPlayerChosenCharacters method will be called.
	 * 
	 * @note This method can be overriden in derived classes to add additional
	 * functionality specific to the game mode.
	 */
	virtual void BeginPlay() override;
};
