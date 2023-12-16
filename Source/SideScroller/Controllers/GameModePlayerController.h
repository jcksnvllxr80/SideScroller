// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameModePlayerController.generated.h"

/**
 * @class APC_PlayerFox
 *
 * @brief The APC_PlayerFox class represents the player character in a side-scroller game.
 *
 * This class inherits from the ABasePaperCharacter class and implements the IProjectileInterface.
 * It provides functionality for handling player interactions, keeping track of player stats such as points, lives,
 * and collectibles, managing spectators, and handling player death and level completion.
 */
class APC_PlayerFox;
/**
 * @class AGameModePlayerController
 *
 * The AGameModePlayerController class is a subclass of APlayerController
 * and is responsible for managing player related operations and interactions
 * within the game mode.
 *
 * It provides functionalities related to player spawning, checking game start requirements,
 * starting levels, and traveling to different levels.
 */
UCLASS()
class SIDESCROLLER_API AGameModePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/**
	 * Spawns a player with the given player blueprint, player color string, and player controller.
	 *
	 * @param PlayerBP The player blueprint to spawn.
	 * @param PlayerColorStr The color string for the player.
	 * @param PlayerController The player controller to assign to the spawned player.
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void SpawnPlayer(
		TSubclassOf<class APC_PlayerFox> PlayerBP,
		const FString& PlayerColorStr,
		APlayerController* PlayerController
	);

	/**
	 * \brief Checks whether the game meet the requirements to start.
	 *
	 * This method is annotated with Unreal Engine macros to make it callable from Blueprint,
	 * executed on the server, reliably replicated, and validated.
	 *
	 * \param None.
	 *
	 * \return None.
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void CheckGameStartReqs();

	/**
	 * @brief Starts a new level.
	 *
	 * This method is used to start a new level in the game.
	 * It is a server-side function that is reliable and requires validation.
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void StartLevel();

	/**
	 * @brief Starts the next level.
	 *
	 * This method is a BlueprintCallable function that can be called from Blueprint graphs.
	 * It is a Server-side function that is executed on the server and replicated to all clients.
	 * The function is Reliable, meaning that it guarantees delivery to all clients.
	 * It is also validated before execution.
	 *
	 * @param None No parameters are required for this method.
	 *
	 * @remarks This method should be used to transition to the next level in the game.
	 * It is recommended to call this function from a game mode or level blueprint when the conditions for
	 * transitioning to the next level are met. Only the server can execute this function and the changes
	 * will be replicated to all clients.
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void StartNextLevel();

	/**
	 * @brief Travel to a different level.
	 *
	 * This method allows the player to travel to a different level within the game.
	 * It is a BlueprintCallable function that should be called on the server.
	 * It is reliable and has validation, ensuring that the travel request is properly handled.
	 */
	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void TravelToLevel();

private:
	/**
	 * @brief PlayerSpawnDropInHeight represents the drop-in height for the player spawn location.
	 *
	 * The drop-in height determines the z-coordinate offset from the ground where the player will spawn.
	 * By default, the player will spawn at a height of 50 units above the ground.
	 *
	 * @note This variable can be edited anywhere.
	 */
	UPROPERTY(EditAnywhere)
	FVector PlayerSpawnDropInHeight {0.f, 0.f, 50.f};

protected:
	/**
	 * @brief Function called when the game starts or when spawned.
	 *
	 * This function is called by the engine to initialize the player controller when the game starts.
	 * It is an override of the BeginPlay() function defined in the parent class AController.
	 *
	 * This function sets the mouse cursor to be hidden and the input mode to be game-only.
	 * It uses the SetShowMouseCursor() and SetInputMode() functions of the APlayerController class.
	 *
	 * @note This function should be declared as virtual in the derived class.
	 */
	virtual void BeginPlay() override;
};
