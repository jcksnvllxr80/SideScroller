// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SideScrollerGameState.h"
#include "LevelGameState.generated.h"

/**
 * ALevelGameState is a subclass of ASideScrollerGameState.
 * It represents the game state for a level in a side-scrolling game.
 */
UCLASS()
class SIDESCROLLER_API ALevelGameState : public ASideScrollerGameState
{
	GENERATED_BODY()

public:
	/**
	 * @brief Opens the respawn menu.
	 *
	 * This method is responsible for opening the respawn menu. It calls the `RespawnLoadMenu` method of the
	 * `USideScrollerGameInstance` instance obtained from the current game instance. If the game instance is
	 * not found, a warning message is printed to the log.
	 */
	UFUNCTION(BlueprintCallable)
	void OpenRespawnMenu();

	/**
	 * @brief Opens the in-game menu.
	 *
	 * This method is responsible for opening the in-game menu. It is a blueprint callable function.
	 * If the `GameInstance` is found, it calls the `InGameLoadMenu` method of the `USideScrollerGameInstance` class.
	 * If the `GameInstance` is not found, it logs a warning message.
	 */
	UFUNCTION(BlueprintCallable) 
	void OpenInGameMenu();
};
