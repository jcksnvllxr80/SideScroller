// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerFoxState.generated.h"

/**
 * @class APlayerFoxState
 *
 * @brief A subclass of APlayerState representing the state of a player character.
 */
UCLASS()
class SIDESCROLLER_API APlayerFoxState : public APlayerState
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief Gets the value of whether the player has chosen a character.
	 *
	 * @return True if the player has chosen a character, false otherwise.
	 */
	UFUNCTION(BlueprintCallable)
	bool GetHasChosenCharacter() const;

	/**
	 * Sets whether the player has chosen a character.
	 *
	 * This function is used to set the value of the variable bHasChosenCharacter.
	 *
	 * @param HasChosenChar A boolean value indicating whether the player has chosen a character.
	 */
	UFUNCTION(BlueprintCallable)
	void SetHasChosenCharacter(bool HasChosenChar);

private:
	/**
	 * @brief A flag indicating whether a character has been selected.
	 *
	 * This flag is used to track whether a character has been chosen by the player. If the flag is set to true,
	 * it means that a character has been selected. By default, the value of this flag is false.
	 */
	UPROPERTY()
	bool bHasChosenCharacter = false;
};
