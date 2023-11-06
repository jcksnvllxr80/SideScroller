// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFoxState.h"

bool APlayerFoxState::GetHasChosenCharacter() const
{
	return this->bHasChosenCharacter;
}

void APlayerFoxState::SetHasChosenCharacter(const bool HasChosenChar)
{
	this->bHasChosenCharacter = HasChosenChar;
}
