// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"
#include "GameFramework/GameModeBase.h"
#include "SideScrollerGameModeBase.generated.h"

/**
 * @class ASideScrollerGameModeBase
 * @brief This class represents the game mode for a side-scrolling game.
 *
 * The ASideScrollerGameModeBase class is a derived class of AGameModeBase and
 * provides functionality specific to a side-scrolling game. It controls the
 * game logic, player management, and other related features.
 */
UCLASS()
class SIDESCROLLER_API ASideScrollerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	/**
	 * Initializes a new instance of the ASideScrollerGameModeBase class.
	 *
	 * @param None.
	 * @return None.
	 */
	ASideScrollerGameModeBase();

	/**
	 * \brief This method is called when the game starts.
	 *
	 * \details It is a virtual method that is part of Unreal Engine's gameplay framework.
	 *          The method is overridden to provide custom behavior at the beginning of the game.
	 *
	 * \param None.
	 *
	 * \return None.
	 */
	virtual void BeginPlay() override;

	/**
	 * @brief Quits the game forcefully.
	 *
	 * This method is used to exit the game immediately without any prompt or confirmation dialog.
	 */
	void QuitGameHard() const;

	/**
	 * @brief Handles the ticking of the game mode.
	 *
	 * This method is responsible for handling the ticking of the game mode. It is called every frame and should
	 * be used to update the state of the game mode.
	 *
	 * @param DeltaTime The time since the last tick.
	 *
	 * @return None.
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * \brief Add a player to the game.
	 *
	 * This function adds a player to the game mode. The player fox passed as parameter is added to the Players array.
	 *
	 * \param PlayerFox The player fox to be added.
	 * \return None.
	 */
	UFUNCTION(BlueprintCallable, Category = Game)
	void AddPlayer(APC_PlayerFox* PlayerFox);

	/**
	 * Removes a player from the game.
	 *
	 * @param PlayerFox The player to remove.
	 *
	 * @note If the specified player is present in the `Players` array, it will be removed. If the player
	 * is not found in the array, a warning message will be logged.
	 */
	UFUNCTION(BlueprintCallable, Category = Game)
	void RemovePlayer(APC_PlayerFox* PlayerFox);

	/**
	 * @brief The variable representing the background music for the game.
	 *
	 * This variable is of type `USoundBase*` and is used to store the background music that will be played
	 * during gameplay. It is an editable property that can be modified in the editor, and is also accessible
	 * for reading in blueprint scripts. By default, the value is set to `nullptr`, meaning no background
	 * music is set initially.
	 *
	 * @see USoundBase
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Audio)
	USoundBase* BackgroundMusic = nullptr;

	/**
	 * @brief Get all players in the game
	 *
	 * This function returns an array of APC_PlayerFox* representing all the players in the game.
	 *
	 * @return An array of APC_PlayerFox* representing all the players in the game
	 */
	UFUNCTION(BlueprintCallable, Category = Game)
	TArray<APC_PlayerFox*> GetPlayers() const;

	/**
	 * TravelToGameOverMenu
	 *
	 * Triggers seamless travel to the Game Over menu level.
	 *
	 * This method sets the bUseSeamlessTravel flag to true and calls the ServerTravel function to
	 * initiate seamless travel to the Game Over menu level. The level path is specified as
	 * "/Game/Maps/Map_GameOverMenu?listen".
	 *
	 * Usage:
	 *     TravelToGameOverMenu();
	 *
	 * @param None
	 * @return None
	 */
	UFUNCTION(BlueprintCallable, Category = Travel)
	void TravelToGameOverMenu();

	/**
	 * Logout method to handle player logout.
	 *
	 * @param Exiting The controller of the player who is logging out.
	 */
	UFUNCTION()
	virtual void Logout(AController* Exiting) override;
	
private:
	/**
	 * @brief TArray containing all the player instances of type APC_PlayerFox.
	 *
	 * This TArray is used to store and manage all the player instances of type APC_PlayerFox. The instances
	 * can be accessed and modified through this variable. Any changes made to this TArray will affect the
	 * player instances stored within it.
	 *
	 * @details
	 * - The variable name is "Players".
	 * - It is a UPROPERTY with the EditAnywhere specifier, allowing it to be modified in the Unreal Editor.
	 * - It belongs to the "Game" category, which helps in organizing variables in the editor.
	 * - It is of type TArray<APC_PlayerFox*>, indicating that it is an array holding pointers to objects
	 * of type APC_PlayerFox.
	 *
	 * @see APC_PlayerFox
	 */
	UPROPERTY(EditAnywhere, Category = Game)
	TArray<APC_PlayerFox*> Players;

	/**
	 * @brief Prints the list of players.
	 *
	 * This method prints the list of players by iterating through the `Players` array. It checks if the array is
	 * empty and returns early if it is. Otherwise, it constructs a string representation of the players'
	 * names and whether they are alive or dead. The final list of players is logged to the console using
	 * the `UE_LOG` macro.
	 */
	UFUNCTION(BlueprintCallable, Category = Players)
	void PrintPlayersList();

protected:
	/**
	 * Enables game mode input for a given player.
	 *
	 * @param NewPlayer The player controller to enable input for.
	 *
	 * @note This method sets the input mode to game-only and hides the mouse cursor for the specified player.
	 */
	static void EnablePlayerGameModeInput(APlayerController* NewPlayer);
};
