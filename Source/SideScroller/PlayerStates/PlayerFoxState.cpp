// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFoxState.h"

/**
 * @brief Get the value of bHasChosenCharacter.
 *
 * @return true if the character has been chosen, false otherwise.
 */
bool APlayerFoxState::GetHasChosenCharacter() const
{
	return this->bHasChosenCharacter;
}

/**
 * Sets the value of bHasChosenCharacter which indicates whether or not
 * the player has chosen a character.
 *
 * @param HasChosenChar - The new value for bHasChosenCharacter
 */
void APlayerFoxState::SetHasChosenCharacter(const bool HasChosenChar)
{
	this->bHasChosenCharacter = HasChosenChar;
}
