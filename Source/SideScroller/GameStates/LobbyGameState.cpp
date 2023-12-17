// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameState.h"

#include "SideScroller/SideScrollerGameInstance.h"
#include "SideScroller/Characters/Players/PC_PlayerFox.h"

/**
 * BeginPlay method called when the game state starts.
 * It sets a timer to spawn player character select widgets after a delay.
 *
 * @param None
 * @return None
 */
void ALobbyGameState::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		this->CharacterSelectDelayTimerHandle,
		this,
		&ALobbyGameState::PlayerSpawnCharacterSelect,
		this->CharacterSelectDelayTimer,
		false
	);
}

/**
 * Spawns the character selection menu for the player.
 *
 * This function is called when a player spawns in the lobby state and has not already chosen a character.
 * If the game instance is not available, a warning message is logged and the function returns early.
 * If a player controller is found, the function checks if the player has already chosen a character.
 * If the player has not chosen a character, the character selection menu is opened.
 * If the player has previously chosen a character, a warning message is logged.
 * If a player controller cannot be found, a warning message is logged.
 *
 * @param None
 * @return None
 * @note This function assumes there is a valid game instance and player controller available.
 */
void ALobbyGameState::PlayerSpawnCharacterSelect()
{
	USideScrollerGameInstance* GameInstance = dynamic_cast<USideScrollerGameInstance*>(GetGameInstance());
	if (GameInstance == nullptr)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("ALobbyGameState::PlayerSpawnCharacterSelect - Cant find GameInstance!")
		);
		return;  // early return
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (PlayerController != nullptr)
	{
		// if the player hasn't selected and stored a character yet, open the character select window
		if (GameInstance->GetChosenCharacter(PlayerController) == nullptr)
		{
			OpenSelectCharacterMenu();
		}
		else
		{
			UE_LOG(LogTemp, Warning,
				TEXT("ALobbyGameState::PlayerSpawnCharacterSelect - Player has previously chosen a character!")
			);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning,
			TEXT("ALobbyGameState::PlayerSpawnCharacterSelect - Cant find PlayerController!")
		);
	}
}

/**
 * Opens the select character menu.
 *
 * @param None
 * @return None
 */
void ALobbyGameState::OpenSelectCharacterMenu()
{
	const ALobbyGameState* LobbyGameState = dynamic_cast<ALobbyGameState*>(GetWorld()->GetGameState());
	if (LobbyGameState != nullptr)
	{
		USideScrollerGameInstance* GameInstance = dynamic_cast<USideScrollerGameInstance*>(GetGameInstance());
		if (GameInstance != nullptr) {
			GameInstance->SelectCharacterLoadMenu();
		} else {
			UE_LOG(LogTemp, Warning,
				TEXT("ALobbyGameState::OpenSelectCharacterMenu - Cant find GameInstance!")
			);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning,
			TEXT("ALobbyGameState::OpenSelectCharacterMenu - Cant find LobbyGameState!")
		);
	}
}
