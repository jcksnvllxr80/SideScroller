// Fill out your copyright notice in the Description page of Project Settings.

#include "SideScrollerGameState.h"

/**
 * BeginPlay method called when the game starts or when the world is loaded.
 *
 * This method is overriden from the parent class and is called when the game starts or when the world is loaded.
 * It should be used to initialize any game state variables or perform any setup tasks.
 *
 * @param None
 *
 * @return None
 */
void ASideScrollerGameState::BeginPlay()
{
	Super::BeginPlay();
	
}

/**
 * @brief Get the current level.
 *
 * This method returns the current level as an integer.
 *
 * @return The current level as an integer.
 */
int ASideScrollerGameState::GetCurrentLevel() const
{
	return CurrentLevel;
}
