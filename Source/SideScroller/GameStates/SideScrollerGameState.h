// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SideScrollerGameState.generated.h"

/**
 * @class ASideScrollerGameState
 *
 * @brief Represents the game state for the side-scroller game.
 *
 * ASideScrollerGameState is a class derived from AGameStateBase, which is used to manage the state of the game.
 * It provides functions and properties for accessing and manipulating the game state, such as the current level.
 *
 * Usage:
 * 1. Create an instance of ASideScrollerGameState in your game.
 * 2. Use the provided functions to get and set the game state values.
 *
 * Example:
 *   ASideScrollerGameState* GameState = GetWorld()->GetGameState<ASideScrollerGameState>();
 *   int CurrentLevel = GameState->GetCurrentLevel();
 *
 * @see AGameStateBase
 */
UCLASS()
class SIDESCROLLER_API ASideScrollerGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	/**
	 * @brief Gets the current level.
	 *
	 * @return The current level as an integer.
	 */
	UFUNCTION(BlueprintCallable)
	int GetCurrentLevel() const;
	
protected:
	/**
	 * @brief Called when the game starts or when spawned.
	 *
	 * This method is called when the game starts or when the actor of the class is spawned. It is a virtual
	 * method that can be overridden in child classes to add custom initialization logic.
	 */
	virtual void BeginPlay() override;

	/**
	 * @brief The CurrentLevel variable represents the current level of the game.
	 *
	 * The CurrentLevel variable is an integer that stores the current level of the game.
	 * It is a UPROPERTY, which means it can be accessed and modified by other components or scripts.
	 * By default, the CurrentLevel is set to 0.
	 */
	UPROPERTY()
	int CurrentLevel = 0;
};
