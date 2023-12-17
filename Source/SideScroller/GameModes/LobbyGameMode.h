// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SideScrollerGameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * ALobbyGameMode is a subclass of ASideScrollerGameModeBase that manages the lobby logic for the game.
 */
UCLASS()
class SIDESCROLLER_API ALobbyGameMode : public ASideScrollerGameModeBase
{
	GENERATED_BODY()

public:
	/**
	 * Logs the current player count in the lobby.
	 *
	 * @param FunctionName The name of the calling function to be displayed in the log.
	 *
	 * @note This method uses the Unreal Engine logging system to output the player count information.
	 */
	UFUNCTION(BlueprintCallable)
	void LogPlayerCount(FString FunctionName) const;

	/**
	 * @brief Starts the game by transitioning from the lobby to the game level.
	 *
	 * This function sets the "SetReadyToStartGame" flag to false in the current game instance and logs a leaving
	 * lobby message. It then retrieves the current world and enables seamless travel. Finally, it constructs the
	 * travel URL to the desired game level and initiates the server travel to that level.
	 */
	UFUNCTION(BlueprintCallable)
	void StartGame();

	/**
	 * @brief Checks if the number of players requirement is fulfilled.
	 *
	 * @return true if the number of players requirement is fulfilled, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool IsNumPlayersRequirementFulfilled() const;

	/**
	 * Sets the minimum number of players required to start the game.
	 */
	UFUNCTION(BlueprintCallable)
	void SetNumPlayersToStart();

	/**
	 * Called when a new player logs in.
	 *
	 * @param NewPlayer The player controller of the new player.
	 *
	 * @return None.
	 */
	virtual void PostLogin(APlayerController* NewPlayer) override;
	/**
	 * @brief Logout method in the ALobbyGameMode class.
	 *
	 * This method is called when a player logs out of the game.
	 * It overrides the Logout method from the AGameModeBase class.
	 *
	 * @param Exiting The controller of the player who is logging out.
	 */
	virtual void Logout(AController* Exiting) override;

protected:
	/**
	 * \brief Initializes the game mode.
	 *
	 * This method is called when the game mode begins play. It is used to initialize any necessary components
	 * or variables.
	 */
	virtual void BeginPlay() override;

private:
	/**
	 * @brief The number of players in the game.
	 *
	 * This variable represents the number of players participating in the game.
	 * The value of this variable should be set before the game starts, indicating
	 * the number of players that will be playing the game.
	 *
	 * The initial value of this variable is 0, indicating that no players are
	 * currently participating in the game.
	 */
	int NumberOfPlayers = 0;
	/**
	 * @var MinPlayersToStartGame
	 * @brief The minimum number of players required to start the game.
	 *
	 * This variable represents the minimum number of players that need to be present in order
	 * to start the game. If the number of players is less than this value, the game cannot be
	 * initiated and the players need to wait until more players join the game.
	 */
	int MinPlayersToStartGame = 6;
	/**
	 * @brief Boolean variable to indicate if the number of players requirement is fulfilled.
	 *
	 * This variable is used to check whether the requirement of having a certain number of players is fulfilled
	 * or not. It is initialized to false by default, indicating that the requirement is not fulfilled.
	 * It should be set to true when the condition of having the required number of players is met.
	 *
	 * @see SetNumPlayersRequirementFulfilled()
	 * @see IsNumPlayersRequirementFulfilled()
	 */
	bool bNumPlayersRequirementFulfilled = false;
};
